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
    std::vector<Pile> piles; // 4 piles: 2 asc, 2 desc
    std::vector<Card> hand;
    std::vector<PlayerInfo> otherPlayers;
    int deckSize;
    int currentPlayers;
    int maxPlayers;
    bool isMyTurn;
    std::string currentPlayerName;
    std::string status; // "waiting", "playing", "finished"
    bool won = false; 
    std::vector<std::string> chatMessages;
};
