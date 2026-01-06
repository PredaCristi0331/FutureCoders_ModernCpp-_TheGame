#include "TestRunner.h"
#include "TestGameLogic.h"

#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <numeric>
#include <optional>
#include <iostream>
#include <set>
#include <memory>
#include <ranges>
#include <bit>
#include <compare>
#include <concepts>
#include <stdexcept>
#include <future>
#include <thread>
#include <regex>
#include <unordered_set>

static bool canPlayAscending(int top, int candidate) {
    return candidate > top || candidate == top - 10;
}
static bool canPlayDescending(int top, int candidate) {
    return candidate < top || candidate == top + 10;
}
static std::vector<int> make_range(int a, int b) {
    std::vector<int> v;
    for (int i = a; i <= b; ++i) v.push_back(i);
    return v;
}

// small value type with defaulted spaceship operator (C++20)
struct TopView {
    int v;
    auto operator<=>(const TopView&) const = default;
};

// generic move consumer to show move semantics
static std::vector<int> consume_and_sort(std::vector<int> v) {
    std::ranges::sort(v);
    return v;
}

// 1..25 existing tests (kept unchanged) ------------------------------------

REGISTER_TEST(TG_test_01) {
    assert(canPlayAscending(1, 2));
    assert(!canPlayAscending(50, 40));
}

REGISTER_TEST(TG_test_02) {
    assert(canPlayDescending(100, 99));
    assert(!canPlayDescending(60, 70));
}

REGISTER_TEST(TG_test_03) {
    assert(canPlayAscending(45, 35));
}

REGISTER_TEST(TG_test_04) {
    assert(canPlayDescending(40, 50));
}

REGISTER_TEST(TG_test_05) {
    int players = 3;
    int minPlay = 2;
    const std::vector<std::vector<int>> hands = { {10,20,30},{15,25,35},{40,50,60} };
    for (const auto& h : hands) {
        assert((int)h.size() >= minPlay);
    }
}

REGISTER_TEST(TG_test_06) {
    std::vector<int> deck = make_range(2, 99);
    std::shuffle(deck.begin(), deck.end(), std::mt19937{ 42 });
    std::vector<int> hand = { 10,20,30 };
    int played = 2;
    for (int i = 0; i < played && !deck.empty(); ++i) {
        hand.push_back(deck.back()); deck.pop_back();
    }
    assert((int)hand.size() == 5);
}

REGISTER_TEST(TG_test_07) {
    std::vector<int> deck;
    int minPlay = deck.empty() ? 1 : 2;
    assert(minPlay == 1);
}

REGISTER_TEST(TG_test_08) {
    int remaining = 0;
    assert(remaining == 0);
}

REGISTER_TEST(TG_test_09) {
    auto canPlayerPlayMin = [](const std::vector<int>& hand, const std::vector<int>& tops, int minPlay) {
        int playable = 0;
        for (int card : hand) {
            for (int t : tops) {
                if (canPlayAscending(t, card) || canPlayDescending(t, card)) {
                    ++playable;
                    break;
                }
            }
        }
        return playable >= minPlay;
        };
    std::vector<int> tops = { 10, 100, 20, 90 };
    std::vector<int> hand = { 11, 12 };
    assert(canPlayerPlayMin(hand, tops, 2));
}

REGISTER_TEST(TG_test_10) {
    std::vector<int> tops = { 1,1,100,100 };
    std::vector<int> hand = { 2,3,4 };
    int played = 0;
    for (size_t i = 0; i < hand.size() && played < 2; ++i) {
        for (size_t p = 0; p < tops.size(); ++p) {
            if (canPlayAscending(tops[p], hand[i]) || canPlayDescending(tops[p], hand[i])) {
                ++played; tops[p] = hand[i]; break;
            }
        }
    }
    assert(played >= 2);
}

REGISTER_TEST(TG_test_11) {
    std::unique_ptr<std::vector<int>> tops = std::make_unique<std::vector<int>>(std::initializer_list<int>{45, 100, 1, 80});
    std::vector<int> hand = { 35,70,90 };
    bool ok = canPlayAscending((*tops)[0], hand[0]);
    assert(ok);
}

REGISTER_TEST(TG_test_12) {
    TopView a{ 20 }, b{ 18 };
    assert((a > b) == true);
}

REGISTER_TEST(TG_test_13) {
    auto remaining_after_deal = [](int players) {
        int per = (players == 2 ? 8 : players == 3 ? 7 : 6);
        int dealt = per * players;
        return 98 - dealt;
        };
    assert(remaining_after_deal(2) == 98 - 16);
}

REGISTER_TEST(TG_test_14) {
    int piles = 4;
    int cardsToPlay = 2;
    int ways = 1;
    for (int i = 0; i < cardsToPlay; ++i) ways *= piles;
    assert(ways == 16);
}

REGISTER_TEST(TG_test_15) {
    std::vector<int> deck = make_range(2, 99);
    std::unordered_set<int> seen(deck.begin(), deck.end());
    assert(seen.size() == deck.size());
}

REGISTER_TEST(TG_test_16) {
    const auto a = consume_and_sort(std::vector<int>{1, 2, 3, 4, 5});
    assert(a.size() == 5);
}

REGISTER_TEST(TG_test_17) {
    int top = 30;
    int card = 31;
    if (canPlayAscending(top, card)) top = card;
    assert(top == 31);
}

REGISTER_TEST(TG_test_18) {
    assert(canPlayAscending(45, 35));
    assert(!canPlayAscending(45, 34));
}

REGISTER_TEST(TG_test_19) {
    assert(canPlayDescending(60, 70));
    assert(!canPlayDescending(60, 71));
}

REGISTER_TEST(TG_test_20) {
    std::vector<int> deck;
    int minPlay = deck.empty() ? 1 : 2;
    assert(minPlay == 1);
}

REGISTER_TEST(TG_test_21) {
    std::vector<int> deck = make_range(2, 99);
    int rounds = 0;
    while (!deck.empty() && rounds < 1000) {
        deck.pop_back();
        ++rounds;
    }
    assert(rounds == 98);
}

REGISTER_TEST(TG_test_22) {
    int top = 45;
    int card = 35;
    if (canPlayAscending(top, card)) top = card;
    assert(top == 35);
}

REGISTER_TEST(TG_test_23) {
    int placed = 98;
    assert(placed >= 98);
}

REGISTER_TEST(TG_test_24) {
    std::vector<int> tops = { 50,50,50,50 };
    std::vector<int> hand = { 1 };
    bool can = false;
    for (int t : tops) if (canPlayAscending(t, hand[0]) || canPlayDescending(t, hand[0])) can = true;
    assert(!can);
}

REGISTER_TEST(TG_test_25) {
    std::vector<int> tops = { 10,100,1,90 };
    std::vector<int> hand;
    for (int i = 2; i <= 99; ++i) hand.push_back(i);
    bool found = std::ranges::any_of(hand, [&](int c) {
        return std::ranges::any_of(tops, [&](int t) { return canPlayAscending(t, c) || canPlayDescending(t, c); });
        });
    assert(found);
}

// ---------------------------------------------------------------------------
// New tests 26..35 - game-specific exception handling, invalid move, draw underflow,
// async move processing, transactional dealing rollback, regex validation
// ---------------------------------------------------------------------------

// helper: throws if move illegal
static void play_card_or_throw(int top, int card) {
    if (!(canPlayAscending(top, card) || canPlayDescending(top, card))) {
        throw std::invalid_argument("illegal move");
    }
}

// helper: draw from deck or throw underflow
static int draw_from_deck(std::vector<int>& deck) {
    if (deck.empty()) throw std::out_of_range("deck empty");
    int v = deck.back(); deck.pop_back(); return v;
}

// 26: illegal move throws
REGISTER_TEST(TG_test_26) {
    bool caught = false;
    try { play_card_or_throw(50, 25); }
    catch (const std::invalid_argument& e) { caught = true; }
    assert(caught);
}

// 27: drawing from empty deck throws
REGISTER_TEST(TG_test_27) {
    std::vector<int> deck;
    bool thrown = false;
    try { draw_from_deck(deck); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}

// 28: transactional dealing (rollback on exception)
REGISTER_TEST(TG_test_28) {
    auto transactional_deal = [](std::vector<int>& deck, std::vector<int>& hand, int n) {
        auto backup_deck = deck;
        try {
            for (int i = 0; i < n; ++i) {
                if (deck.empty()) throw std::runtime_error("not enough");
                hand.push_back(deck.back()); deck.pop_back();
            }
        }
        catch (...) {
            deck = std::move(backup_deck);
            throw;
        }
        };
    std::vector<int> deck = make_range(2, 5);
    std::vector<int> hand;
    bool rolled = false;
    try { transactional_deal(deck, hand, 10); }
    catch (...) { rolled = true; }
    assert(rolled && hand.empty());
}

// 29: async play simulation - exceptions propagate via future
REGISTER_TEST(TG_test_29) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("play fail"); return 0; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error& e) { catched = std::string(e.what()) == "play fail"; }
    assert(catched);
}

// 30: regex validate pile input (game frontend)
REGISTER_TEST(TG_test_30) {
    std::string input = "pile:3";
    std::regex re(R"(pile:(\d+))");
    std::smatch m;
    bool ok = std::regex_match(input, m, re);
    assert(ok && m.size() == 2 && m[1] == "3");
}

// 31: ensure TopView three-way compare works in sort and exceptions not thrown
REGISTER_TEST(TG_test_31) {
    std::vector<TopView> v = { {2},{1},{3} };
    std::ranges::sort(v);
    assert(v[0].v == 1);
}

// 32: ensure move semantics for deck consumption (moved deck is empty)
REGISTER_TEST(TG_test_32) {
    auto deck = make_range(2, 10);
    auto moved_deck = std::move(deck);
    assert(deck.empty() && !moved_deck.empty());
}



// 34: detect duplicate top values using unordered_set - no exception but logic check
REGISTER_TEST(TG_test_34) {
    std::vector<int> tops = { 1,1,100,100 };
    std::unordered_set<int> s(tops.begin(), tops.end());
    assert(s.size() == 2);
}

// 35: stress small concurrent simulation: spawn threads that pop deck concurrently with mutex
REGISTER_TEST(TG_test_35) {
    std::vector<int> deck = make_range(2, 50);
    std::mutex m;
    auto worker = [&](int take) {
        int got = 0;
        while (got < take) {
            std::lock_guard<std::mutex> lk(m);
            if (deck.empty()) break;
            deck.pop_back();
            ++got;
        }
        };
    std::thread t1(worker, 10), t2(worker, 10);
    t1.join(); t2.join();
    assert(deck.size() <= 50 && deck.size() >= 30);
}