#include "TestPlayer.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <cassert>
#include <optional>

void testPlayerTurnOrder() {
    std::vector<std::string> players{ "Ana","Bob","Carmen" };
    size_t turn = 0;
    assert(players[turn % players.size()] == "Ana");
    turn++;
    assert(players[turn % players.size()] == "Bob");
    turn++;
    assert(players[turn % players.size()] == "Carmen");
    turn++;
    assert(players[turn % players.size()] == "Ana");
    std::cout << "Player turn order passed\n";
}

void testPlayerScoreCompute() {
    std::vector<int> cardsLeftPerLose{ 5, 2, 3 };
    double avg = std::accumulate(cardsLeftPerLose.begin(), cardsLeftPerLose.end(), 0) / double(cardsLeftPerLose.size());
    assert(avg == 3.3333333333333335);
    std::cout << "Player score compute passed\n";
}

void testPlayerPerformanceScore() {
    auto performanceScore = [](int gamesPlayed, int gamesWon, int cardsLeftAtLose) -> int {
        if (gamesPlayed == 0) return 1;
        double winRatio = double(gamesWon) / gamesPlayed;
        double penalty = double(cardsLeftAtLose) / (gamesPlayed - gamesWon + 1);
        int score = int(1 + 4 * winRatio - penalty / 20);
        if (score > 5) score = 5;
        if (score < 1) score = 1;
        return score;
        };
    assert(performanceScore(10, 8, 20) >= 4);
    assert(performanceScore(5, 0, 40) == 1);
    std::cout << "Player performance score passed\n";
}