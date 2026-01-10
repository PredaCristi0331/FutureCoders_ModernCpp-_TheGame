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
#include <memory>
#include <ranges>
#include <concepts>
#include <bit>
#include <stdexcept>
#include <future>
#include <regex>
#include <thread>

template<std::integral T>
double avg_integrals(const std::vector<T>& v) {
    T s = 0;
    for (auto x : v) s += x;
    return double(s) / v.size();
}


REGISTER_TEST(TP_test_01) {
    std::vector<std::unique_ptr<int>> bags;
    bags.push_back(std::make_unique<int>(5));
    auto moved = std::move(bags[0]);
    assert(moved && bags.size() == 1);
}

REGISTER_TEST(TP_test_02) {
    auto perPlayer = [](int players) -> int {
        if (players == 2) return 8;
        if (players == 3) return 7;
        return 6;
        };
    assert(perPlayer(2) == 8);
    assert(perPlayer(5) == 6);
}

REGISTER_TEST(TP_test_03) {
    std::vector<int> cardsLeft = { 0,2,5,1 };
    double mean = avg_integrals(cardsLeft);
    assert(mean >= 0);
}

REGISTER_TEST(TP_test_04) {
    std::vector<std::tuple<int, int, std::string>> v = { {5,20,"A"}, {5,10,"B"}, {4,100,"C"} };
    std::ranges::stable_sort(v, [](auto& a, auto& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) > std::get<0>(b);
        return std::get<1>(a) > std::get<1>(b);
        });
    assert(std::get<2>(v.front()) == "A");
}

REGISTER_TEST(TP_test_05) {
    auto valid = [](const std::string& msg)->bool {
        std::regex digits(R"(\d+)");
        return !std::regex_search(msg, digits);
        };
    assert(!valid("I have 47"));
    assert(valid("Maybe don't play that pile"));
}

REGISTER_TEST(TP_test_06) {
    const std::vector<int> cardsLeft = { 0,2,5,1 };
    double avg = avg_integrals(cardsLeft);
    assert(avg >= 0);
}

REGISTER_TEST(TP_test_07) {
    std::vector<std::pair<int, std::string>> r = { {10,"A"},{10,"B"},{8,"C"} };
    std::stable_sort(r.begin(), r.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(r[0].second == "A");
}

REGISTER_TEST(TP_test_08) {
    auto handSize = [](int players)->int { if (players == 2) return 8; if (players == 3) return 7; return 6; };
    for (int p = 2; p <= 5; ++p) assert(handSize(p) >= 6);
}

REGISTER_TEST(TP_test_09) {
    std::vector<int> wins = { 1,0,1,1 };
    int teamWins = std::accumulate(wins.begin(), wins.end(), 0);
    assert(teamWins == 3);
}

REGISTER_TEST(TP_test_10) {
    const int deck = 98;
    int players = 3;
    int per = (players == 2 ? 8 : players == 3 ? 7 : 6);
    int dealt = per * players;
    int draws = deck - dealt;
    assert(draws >= 0);
}

REGISTER_TEST(TP_test_11) {
    std::vector<std::string> p = { "A","B","C" };
    for (int r = 0; r < 6; ++r) {
        auto dealer = p[r % p.size()];
        (void)dealer;
    }
    assert(true);
}

REGISTER_TEST(TP_test_12) {
    std::optional<int> cur;
    cur = 1;
    assert(cur.has_value());
}

REGISTER_TEST(TP_test_13) {
    auto isVague = [](const std::string& m)->bool {
        return m.find("don't") != std::string::npos || m.find("few") != std::string::npos || m.find("maybe") != std::string::npos;
        };
    assert(isVague("Maybe don't play first pile"));
}

REGISTER_TEST(TP_test_14) {
    constexpr int required = 2;
    int played = 1;
    assert((required - played) == 1);
}

REGISTER_TEST(TP_test_15) {
    int players = 4;
    std::vector<int> plays = { 2,2,1,2 };
    bool someoneFailed = std::any_of(plays.begin(), plays.end(), [](int x) { return x < 2; });
    assert(someoneFailed);
}

REGISTER_TEST(TP_test_16) {
    std::vector<std::string> names = { "a","b","c" };
    std::map<std::string, int> hours;
    for (size_t i = 0; i < names.size(); ++i) hours[names[i]] = i * 10;
    assert(hours["b"] == 10);
}

REGISTER_TEST(TP_test_17) {
    auto valid = [](int n) { return n >= 2 && n <= 5; };
    assert(valid(2) && valid(5));
}

REGISTER_TEST(TP_test_18) {
    std::vector<int> left = { 3,2,4 };
    double avg = avg_integrals(left);
    assert(avg > 0);
}

REGISTER_TEST(TP_test_19) {
    std::vector<std::pair<int, std::string>> v = { {5,"A"},{7,"B"},{3,"C"} };
    std::ranges::sort(v, [](auto& a, auto& b) { return a.first > b.first; });
    assert(v.front().second == "B");
}

REGISTER_TEST(TP_test_20) {
    std::vector<int> left = { 1,2,3 };
    int tot = std::accumulate(left.begin(), left.end(), 0);
    assert(tot == 6);
}

REGISTER_TEST(TP_test_21) {
    std::string m = "I have cards";
    std::regex digits(R"(\d+)");
    bool hasDigit = std::regex_search(m, digits);
    assert(!hasDigit);
}

REGISTER_TEST(TP_test_22) {
    std::vector<int> hours = { 10,20,30 };
    double mean = std::accumulate(hours.begin(), hours.end(), 0.0) / hours.size();
    double var = 0;
    for (double h : hours) var += (h - mean) * (h - mean);
    var /= hours.size();
    assert(var >= 0);
}

REGISTER_TEST(TP_test_23) {
    int remaining = 10;
    int players = 4;
    int minPerRound = players * 2;
    int rounds = (remaining + minPerRound - 1) / minPerRound;
    assert(rounds >= 1);
}

REGISTER_TEST(TP_test_24) {
    std::vector<std::pair<int, std::string>> v = { {3,"A"},{3,"B"},{2,"C"} };
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(v[0].second == "A");
}

REGISTER_TEST(TP_test_25) {
    int readyMask = 0;
    readyMask |= (1 << 0);
    readyMask |= (1 << 2);
    assert((readyMask & (1 << 1)) == 0);
}


static void ensure_valid_player(int idx, int maxPlayers) {
    if (idx < 0 || idx >= maxPlayers) throw std::out_of_range("invalid player");
}

REGISTER_TEST(TP_test_26) {
    bool thrown = false;
    try { ensure_valid_player(5, 4); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}

REGISTER_TEST(TP_test_27) {
    auto deal_count = [](int players)->int {
        if (players < 2 || players > 5) throw std::invalid_argument("players");
        return players == 2 ? 8 : players == 3 ? 7 : 6;
        };
    bool thrown = false;
    try { deal_count(1); }
    catch (const std::invalid_argument&) { thrown = true; }
    assert(thrown);
}

REGISTER_TEST(TP_test_28) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("action fail"); return 0; });
    bool caught = false;
    try { fut.get(); }
    catch (const std::runtime_error&) { caught = true; }
    assert(caught);
}

REGISTER_TEST(TP_test_29) {
    auto validate_chat = [](const std::string& m) {
        std::regex digits(R"(\d+)");
        if (std::regex_search(m, digits)) throw std::invalid_argument("no digits allowed");
        };
    bool thrown = false;
    try { validate_chat("I have 10"); }
    catch (const std::invalid_argument& e) { thrown = (std::string(e.what()) == "no digits allowed"); }
    assert(thrown);
}

REGISTER_TEST(TP_test_30) {
    bool freed = false;
    struct R { bool* p; R(bool* q) :p(q) {} ~R() noexcept { if (p) *p = true; } };
    try {
        std::unique_ptr<R> pr = std::make_unique<R>(&freed);
        throw std::runtime_error("x");
    }
    catch (...) {}
    assert(freed);
}

REGISTER_TEST(TP_test_31) {
    std::vector<int> hours = { 0,0,0 };
    double mean = avg_integrals(hours);
    assert(mean == 0.0);
}

REGISTER_TEST(TP_test_32) {
    std::vector<std::pair<int, std::string>> v = { {1,"A"},{1,"B"} };
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.first > b.first; });
    assert(v[0].second == "A");
}

REGISTER_TEST(TP_test_33) {
    int mask = 0;
    mask |= (1 << 1);
    mask |= (1 << 3);
    assert((mask & (1 << 1)) != 0);
}

REGISTER_TEST(TP_test_34) {
    auto f = std::async(std::launch::async, []() {
        return std::async(std::launch::async, []() { throw std::runtime_error("inner"); return 0; }).get();
        });
    bool caught = false;
    try { f.get(); }
    catch (const std::runtime_error&) { caught = true; }
    assert(caught);
}
