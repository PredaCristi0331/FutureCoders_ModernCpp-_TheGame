#include "GameSessionManager.h"
#include "RequestValidator.h"
#include "../Database/Database/DatabaseManager.h"

import GameTable;
import Card;
import Player;
#include <iostream>

namespace http
{
    void FinishGameAndSaveStats(GameSession& session, bool won) {
        if(session.status == "finished") return;
        
        std::cout << "[Referee] Finishing game " << session.gameId << " (Won: " << won << ")" << std::endl;
        session.status = "finished";
        session.won = won;
        
        std::int64_t duration = (session.startTime > 0) ? (std::time(nullptr) - session.startTime) : 0;
        
        try {
            DatabaseManager::finishSession(session.gameId, won, session.table->SizeDeckCards(), 
                                           0, std::to_string(std::time(nullptr)), duration);

            for(int i=0; i<session.currentPlayers; ++i) {
                std::string name = session.playerNames[i];
                auto u = DatabaseManager::getUserByUsername(name);
                if(u) {
                    PlayerGameStats s;
                    s.user_id = u->id;
                    s.game_session_id = session.gameId;
                    s.is_host = (i==0);
                    int cardsInHand = static_cast<int>(session.table->GetGamer(i).GetCards().size());
                    s.final_cards_in_hand = cardsInHand;
                    s.moves_played = 0; 

                    s.won = won && (cardsInHand == 0);
                    DatabaseManager::savePlayerStats(s);
                    DatabaseManager::updateUserStatsIncrement(u->id, s.won, cardsInHand, duration); 
                    std::cout << "[Stats] Player " << name << " (ID: " << u->id << "): won=" << s.won 
                              << ", cardsInHand=" << cardsInHand << ", gameWon=" << won << ", duration=" << duration << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[Referee] DB Error: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "[Referee] Unknown DB Error" << std::endl;
        }
    }

    crow::response GameSessionManager::CreateGame(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto validationResult = http::RequestValidator::ValidateJSON(req);
        if (validationResult)
        {
            return std::move(validationResult.errorResponse);
        }

        auto body = crow::json::load(req.body);
        int maxPlayers = 4;
        
        if (body.has("maxPlayers"))
        {
            maxPlayers = body["maxPlayers"].i();
            if (maxPlayers < 2 || maxPlayers > 5)
            {
                return http::RequestValidator::CreateErrorResponse(400, "Invalid maxPlayers",
                    "maxPlayers must be between 2 and 5");
            }
        }

        GameSession session;
        session.gameId = m_nextGameId++;
        session.maxPlayers = maxPlayers;
        session.currentPlayers = 0;
        session.status = "waiting";
        session.table = std::make_shared<game::GameTable>(maxPlayers);

        DatabaseManager::createWaitingSession(std::to_string(std::time(nullptr)));

        m_sessions[session.gameId] = session;

        crow::json::wvalue response;
        response["gameId"] = session.gameId;
        response["status"] = "created";
        response["maxPlayers"] = maxPlayers;

        return crow::response(200, response);
    }

    crow::response GameSessionManager::JoinGame(int gameId, const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end())
        {
            return http::RequestValidator::CreateErrorResponse(404, "Game not found",
                "No game exists with ID " + std::to_string(gameId));
        }

        GameSession& session = it->second;

        if (session.currentPlayers >= session.maxPlayers)
        {
            return http::RequestValidator::CreateErrorResponse(400, "Game is full",
                "This game has reached maximum capacity");
        }
        
        if (session.status != "waiting") {
             return http::RequestValidator::CreateErrorResponse(400, "Game already started",
                "Cannot join a game in progress");
        }

        auto validationResult = http::RequestValidator::ValidateJSON(req);
        if (validationResult)
        {
            return std::move(validationResult.errorResponse);
        }

        auto body = crow::json::load(req.body);
        std::string playerName = "Anonymous";
        
        if (body.has("playerName"))
        {
            playerName = body["playerName"].s();
        }
        
        session.playerNames.push_back(playerName);
        session.currentPlayers++;
        
        session.table->AddGamer(playerName);

        if (session.currentPlayers == session.maxPlayers) {
            session.status = "playing";
            session.startTime = std::time(nullptr);
            session.table->SetNrGamer(session.currentPlayers);
            session.table->AddInitialCards();
            session.table->MixingDeckCards();
            session.table->IssuerCard();
            session.currentPlayerIndex = 0;
        }

        crow::json::wvalue response;
        response["gameId"] = gameId;
        response["playerName"] = playerName;
        response["currentPlayers"] = session.currentPlayers;
        response["status"] = session.status;
        response["playerIndex"] = session.currentPlayers - 1;

        return crow::response(200, response);
    }

    crow::response GameSessionManager::GetGameInfo(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end())
        {
            return http::RequestValidator::CreateErrorResponse(404, "Game not found",
                "No game exists with ID " + std::to_string(gameId));
        }

        const GameSession& session = it->second;

        crow::json::wvalue response;
        response["gameId"] = session.gameId;
        response["maxPlayers"] = session.maxPlayers;
        response["currentPlayers"] = session.currentPlayers;
        response["status"] = session.status;
        
        std::vector<crow::json::wvalue> players;
        for (const auto& name : session.playerNames) {
            players.push_back(crow::json::wvalue(name));
        }
        response["players"] = std::move(players);

        return crow::response(200, response);
    }

    crow::response GameSessionManager::GetAllGames()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        crow::json::wvalue games;
        games = crow::json::wvalue::list();
        
        int idx = 0;
        for (const auto& [id, session] : m_sessions)
        {
            if (session.status == "waiting")
            {
                crow::json::wvalue game;
                game["gameId"] = session.gameId;
                game["currentPlayers"] = session.currentPlayers;
                game["maxPlayers"] = session.maxPlayers;
                game["status"] = session.status;
                games[idx++] = std::move(game);
            }
        }

        return crow::response(200, games);
    }
    
    crow::response GameSessionManager::StartGame(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        
        GameSession& session = it->second;
        if(session.currentPlayers < 2) return crow::response(400, "Not enough players");
        if(session.status != "waiting") return crow::response(400, "Game already started");

        session.status = "playing";
        session.startTime = std::time(nullptr);
        
        session.table->SetNrGamer(session.currentPlayers);
        session.table->AddInitialCards();
        session.table->MixingDeckCards();
        session.table->IssuerCard();
        
        session.currentPlayerIndex = 0;
        session.cardsPlayedThisTurn = 0;
        
        if (session.table->IsGameLost(0)) {
            FinishGameAndSaveStats(session, false);
        }

        return crow::response(200, "Game started");
    }

    crow::response GameSessionManager::EndGame(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        it->second.status = "finished";
        return crow::response(200, "Game ended");
    }

    crow::response GameSessionManager::ForceWin(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        
        GameSession& session = it->second;
        if(session.status == "finished") return crow::response(200, "Already finished");

        if(session.table) {
            for(int i=0; i<session.currentPlayers; ++i) {
                while(!session.table->GetGamer(i).GetCards().empty()) {
                    auto c = session.table->GetGamer(i).GetCards().back();
                    session.table->RemoveCardFromHand(i, c);
                }
            }
        }
        
        FinishGameAndSaveStats(session, true);
        return crow::response(200, "Game Force Won!");
    }
    
    crow::json::wvalue CardToJson(const game::Card& c) {
        crow::json::wvalue j;
        j["value"] = c.GetCardNumber();
        return j;
    }

    crow::response GameSessionManager::GetGameState(int gameId, int userId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        
        const GameSession& session_const = it->second;
        GameSession& session = it->second; 
        
        if (!session.table) return crow::response(500, "Game table not initialized");

        if (session.status == "playing") {
             bool lost = session.table->IsGameLost(session.currentPlayerIndex);
             
             if (lost) {
                  std::cout << "[Referee] DETECTED LOSS for player " << session.currentPlayerIndex << std::endl;
                  FinishGameAndSaveStats(session, false);
             }
        }

        crow::json::wvalue response;
        response["status"] = session.status;
        response["won"] = session.won;
        response["currentPlayerIndex"] = session.currentPlayerIndex;
        response["currentPlayerName"] = session.playerNames[session.currentPlayerIndex];
        
        response["currentPlayers"] = session.currentPlayers;
        response["maxPlayers"] = session.maxPlayers;
        
        crow::json::wvalue piles;
        if (session.table) {
            response["deckCount"] = session.table->SizeDeckCards();
            piles["inc1"] = session.table->GetLastCardFromIncreasingFirst().GetCardNumber();
            piles["inc2"] = session.table->GetLastCardFromIncreasingSecond().GetCardNumber();
            piles["dec1"] = session.table->GetLastCardFromDecreasingFirst().GetCardNumber();
            piles["dec2"] = session.table->GetLastCardFromDecreasingSecond().GetCardNumber();
        } else {
             response["deckCount"] = 98;
             piles["inc1"] = 1;
             piles["inc2"] = 1;
             piles["dec1"] = 100;
             piles["dec2"] = 100;
        }
        response["piles"] = std::move(piles);
        
        int requestPlayerIndex = userId;
        
        if (requestPlayerIndex >= 0 && requestPlayerIndex < session.currentPlayers) {
            const auto& hand = session.table->GetGamer(requestPlayerIndex).GetCards();
            std::vector<crow::json::wvalue> handJson;
            for(const auto& c : hand) {
                handJson.push_back(CardToJson(c));
            }
            response["myHand"] = std::move(handJson);
        }
        
        std::vector<crow::json::wvalue> handSizes;
        for(int i=0; i<session.currentPlayers; ++i) {
            crow::json::wvalue pinfo;
            pinfo["name"] = session.playerNames[i];
            pinfo["cardCount"] = static_cast<int>(session.table->GetGamer(i).GetCards().size());
            handSizes.push_back(std::move(pinfo));
        }
        response["players"] = std::move(handSizes);

        return crow::response(200, response);
    }

    crow::response GameSessionManager::PlayCard(int gameId, const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        
        GameSession& session = it->second;
        if(session.status != "playing") return crow::response(400, "Game not playing");

        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        
        int playerIndex = body["playerIndex"].i();
        int cardValue = body["cardValue"].i();
        int pileIndex = body["pileIndex"].i();
        
        if (playerIndex != session.currentPlayerIndex) {
             return crow::response(403, "Not your turn");
        }
        
        game::Card card(cardValue);
        
        if(!session.table->GetGamer(playerIndex).HasCard(cardValue)) {
             return crow::response(400, "You do not have this card");
        }
        
        if(!session.table->IsValidMove(card, pileIndex)) {
             return crow::response(400, "Invalid move");
        }
        
        session.table->RemoveCardFromHand(playerIndex, card);
        
        switch(pileIndex) {
            case 1: session.table->PushIncreasingFirst(card); break;
            case 2: session.table->PushIncreasingSecond(card); break;
            case 3: session.table->PushDecreasingFirst(card); break;
            case 4: session.table->PushDecreasingSecond(card); break;
        }
        
        session.cardsPlayedThisTurn++;

        if(session.table->IsGameWon()) {
             FinishGameAndSaveStats(session, true);
             return crow::response(200, "Game Won!");
        }
        
        if (session.table->GetGamer(playerIndex).GetCards().empty()) {
             int targetHandSize = 6;
            if(session.maxPlayers == 2) targetHandSize = 8;
            else if(session.maxPlayers == 3) targetHandSize = 7;
            
            while(session.table->GetGamer(playerIndex).GetCards().size() < targetHandSize && session.table->SizeDeckCards() > 0) {
                 game::Card c = session.table->DeckCardsLast();
                 session.table->RemoveDeckCardsLast();
                 session.table->PushCard(c, playerIndex);
            }
        }

        int minCards = (session.table->SizeDeckCards() > 0) ? 2 : 1;
        
        if (session.cardsPlayedThisTurn < minCards) {
            if (session.table->IsGameLost(session.currentPlayerIndex)) {
                FinishGameAndSaveStats(session, false);
                return crow::response(200, "Blocked before min cards... GAME OVER (Lost)");
            }
        }
        
         return crow::response(200, "Move Accepted");
    }

    crow::response GameSessionManager::EndTurn(int gameId, const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        
        GameSession& session = it->second;
        if(session.status != "playing") return crow::response(400, "Game not playing");

        auto body = crow::json::load(req.body);
        int playerIndex = body["playerIndex"].i();
        
        if (playerIndex != session.currentPlayerIndex) {
             return crow::response(403, "Not your turn");
        }
        
        int minCards = (session.table->SizeDeckCards() > 0) ? 2 : 1;
        if (session.cardsPlayedThisTurn < minCards) {
            return crow::response(400, "Must play at least " + std::to_string(minCards) + " cards.");
        }

        session.cardsPlayedThisTurn = 0;
        session.currentPlayerIndex = (session.currentPlayerIndex + 1) % session.currentPlayers;
        
        if(session.table->IsGameLost(session.currentPlayerIndex)) {
             FinishGameAndSaveStats(session, false);
             return crow::response(200, "Turn ended. Next player blocked... GAME OVER (Lost)");
        }
        
        return crow::response(200, "Turn Ended");
    }
}
