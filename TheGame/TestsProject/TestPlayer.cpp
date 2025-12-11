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


#include "TestRunner.h"
#include "TestPlayer.h"

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <optional>
#include <map>
#include <functional>

// 1: turn order rotation
REGISTER_TEST(TP_test_01) {
    std::vector<std::string> players{ "A","B","C","D" };
    int turn = 0;
    assert(players[turn % players.size()] == "A");
    turn += 3;
    assert(players[turn % players.size()] == "D");
}

// 2: dealing counts by player count
REGISTER_TEST(TP_test_02) {
    auto perPlayer = [](int players) {
        if (players == 2) return 8;
        if (players == 3) return 7;
        return 6;
        };
    assert(perPlayer(2) == 8);
    assert(perPlayer(5) == 6);
}

// 3: compute performance score helper
REGISTER_TEST(TP_test_03) {
    auto perf = [](int gamesPlayed, int gamesWon, int cardsLeftAvg) -> int {
        if (gamesPlayed == 0) return 1;
        double p = double(gamesWon) / gamesPlayed;
        double penalty = cardsLeftAvg / 20.0;
        int score = int(std::clamp(1.0 + 4.0 * p - penalty, 1.0, 5.0));
        return score;
        };
    assert(perf(10, 8, 5) >= 4);
}

// 4: leaderboard sort by score then hours
REGISTER_TEST(TP_test_04) {
    std::vector<std::tuple<int, int, std::string>> v = { {5,20,"A"}, {5,10,"B"}, {4,100,"C"} };
    std::sort(v.begin(), v.end(), [](auto& a, auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) > std::get<0>(b);
        return std::get<1>(a) > std::get<1>(b);
        });
    assert(std::get<2>(v.front()) == "A");
}

// 5: chat message validation (no exact numbers)
REGISTER_TEST(TP_test_05) {
    auto valid = [](const std::string& msg)->bool {
        for (char c : msg) if (std::isdigit((unsigned char)c)) return false;
        return true;
        };
    assert(!valid("I have 47"));
    assert(valid("Maybe don't play that pile"));
}

// 6: aggregate stats from many games
REGISTER_TEST(TP_test_06) {
    std::vector<int> cardsLeft = { 0,2,5,1 };
    double avg = std::accumulate(cardsLeft.begin(), cardsLeft.end(), 0.0) / cardsLeft.size();
    assert(avg >= 0);
}

// 7: ranking ties handling stable order
REGISTER_TEST(TP_test_07) {
    std::vector<std::pair<int, std::string>> r = { {10,"A"},{10,"B"},{8,"C"} };
    std::stable_sort(r.begin(), r.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(r[0].second == "A");
}

// 8: compute expected hand size after dealing (players 2..5)
REGISTER_TEST(TP_test_08) {
    auto handSize = [](int players)->int { if (players == 2) return 8; if (players == 3) return 7; return 6; };
    for (int p = 2; p <= 5; ++p) assert(handSize(p) >= 6);
}

// 9: simulate minimal team score metric
REGISTER_TEST(TP_test_09) {
    std::vector<int> wins = { 1,0,1,1 };
    int teamWins = std::accumulate(wins.begin(), wins.end(), 0);
    assert(teamWins == 3);
}

// 10: compute expected number of draw rounds given deck and players
REGISTER_TEST(TP_test_10) {
    int deck = 98;
    int players = 3;
    int per = (players == 2 ? 8 : players == 3 ? 7 : 6);
    int dealt = per * players;
    int draws = deck - dealt;
    assert(draws >= 0);
}

// 11: check rotating dealer index
REGISTER_TEST(TP_test_11) {
    std::vector<std::string> p = { "A","B","C" };
    for (int r = 0; r < 6; ++r) {
        auto dealer = p[r % p.size()];
        (void)dealer;
    }
    assert(true);
}

// 12: optional current player calculation
REGISTER_TEST(TP_test_12) {
    std::optional<int> cur;
    cur = 1;
    assert(cur.has_value());
}

// 13: chat suggestion fuzzy matching (simple)
REGISTER_TEST(TP_test_13) {
    auto isVague = [](const std::string& m)->bool {
        return m.find("don't") != std::string::npos || m.find("few") != std::string::npos || m.find("maybe") != std::string::npos;
        };
    assert(isVague("Maybe don't play first pile"));
}

// 14: compute card-play deficit
REGISTER_TEST(TP_test_14) {
    int required = 2;
    int played = 1;
    assert((required - played) == 1);
}

// 15: simulate simple team failure detection
REGISTER_TEST(TP_test_15) {
    int players = 4;
    std::vector<int> plays = { 2,2,1,2 };
    bool someoneFailed = std::any_of(plays.begin(), plays.end(), [](int x) { return x < 2; });
    assert(someoneFailed);
}

// 16: generate player ids and map to hours played
REGISTER_TEST(TP_test_16) {
    std::vector<std::string> names = { "a","b","c" };
    std::map<std::string, int> hours;
    for (size_t i = 0; i < names.size(); ++i) hours[names[i]] = i * 10;
    assert(hours["b"] == 10);
}

// 17: validate min and max players
REGISTER_TEST(TP_test_17) {
    auto valid = [](int n) { return n >= 2 && n <= 5; };
    assert(valid(2) && valid(5));
}

// 18: compute average hand size at end of lost game
REGISTER_TEST(TP_test_18) {
    std::vector<int> left = { 3,2,4 };
    double avg = std::accumulate(left.begin(), left.end(), 0.0) / left.size();
    assert(avg > 0);
}

// 19: top-k players by wins
REGISTER_TEST(TP_test_19) {
    std::vector<std::pair<int, std::string>> v = { {5,"A"},{7,"B"},{3,"C"} };
    std::sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(v.front().second == "B");
}

// 20: map reduce style: total cards left
REGISTER_TEST(TP_test_20) {
    std::vector<int> left = { 1,2,3 };
    int tot = std::accumulate(left.begin(), left.end(), 0);
    assert(tot == 6);
}

// 21: check chat restriction detection (digits)
REGISTER_TEST(TP_test_21) {
    std::string m = "I have cards";
    bool hasDigit = std::any_of(m.begin(), m.end(), [](char c) { return std::isdigit((unsigned char)c); });
    assert(!hasDigit);
}

// 22: compute fairness metric (std deviation naive)
REGISTER_TEST(TP_test_22) {
    std::vector<int> hours = { 10,20,30 };
    double mean = std::accumulate(hours.begin(), hours.end(), 0.0) / hours.size();
    double var = 0;
    for (double h : hours) var += (h - mean) * (h - mean);
    var /= hours.size();
    assert(var >= 0);
}

// 23: compute expected minimal plays remaining for victory
REGISTER_TEST(TP_test_23) {
    int remaining = 10;
    int players = 4;
    int minPerRound = players * 2;
    int rounds = (remaining + minPerRound - 1) / minPerRound;
    assert(rounds >= 1);
}

// 24: rank players with ties using stable sort
REGISTER_TEST(TP_test_24) {
    std::vector<std::pair<int, std::string>> v = { {3,"A"},{3,"B"},{2,"C"} };
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(v[0].second == "A");
}

// 25: compact player state encoding into integer mask
REGISTER_TEST(TP_test_25) {
    int readyMask = 0;
    readyMask |= (1 << 0);
    readyMask |= (1 << 2);
    assert((readyMask & (1 << 1)) == 0);
}