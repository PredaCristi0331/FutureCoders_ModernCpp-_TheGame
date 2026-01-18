#pragma once
#include <vector>
#include <string>

struct Card {
    int value;
};

struct Pile {
    bool isAscending;
    int topCardValue;
};

struct PlayerInfo {
    int id;
    std::string name;
    int cardCount;
};

struct GameState {
    int gameId;
    std::vector<Pile> piles; 
    std::vector<Card> hand;
    std::vector<PlayerInfo> otherPlayers;
    int deckSize;
    int currentPlayers;
    int maxPlayers;
    bool isMyTurn;
    std::string currentPlayerName;
    std::string status; 
    bool won = false; 
    std::vector<std::string> chatMessages;
};
