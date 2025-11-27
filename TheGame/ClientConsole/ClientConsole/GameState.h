#pragma once
#include <vector>
#include <string>

struct GameCard {
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
    std::vector<GameCard> hand;
    std::vector<PlayerInfo> otherPlayers;
    int deckSize;
    bool isMyTurn;
    std::vector<std::string> chatMessages;
};
