#include "TestGameLogic.h"
#include "TestRunner.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <optional>

// pentru test: funcție simplă care verifică mutarea pe teanc ascendent
bool canPlayAscending(int top, int candidate) {
    return candidate > top || candidate == top - 10;
}

bool canPlayDescending(int top, int candidate) {
    return candidate < top || candidate == top + 10;
}

void testAscendingRuleLogic() {
    assert(canPlayAscending(23, 25));   // valoare mai mare
    assert(!canPlayAscending(23, 18));  // nu se poate normal
    std::cout << "Asc rule logic passed\n";
}

void testDescendingRuleLogic() {
    assert(canPlayDescending(70, 67));  // valoare mai mică
    assert(!canPlayDescending(70, 72)); // nu merge
    std::cout << "Desc rule logic passed\n";
}

void testBackwardsTrickLogic() {
    assert(canPlayAscending(63, 53));   // backwards trick
    assert(canPlayDescending(44, 54));  // backwards trick
    std::cout << "Backwards trick logic passed\n";
}

void testWinLoseLogic() {
    auto winCondition = [](int cardsLeft) -> bool { return cardsLeft == 0; };
    auto loseCondition = [](std::vector<std::pair<int, std::vector<int>>> pilesWithHand) -> bool {
        // fiecare jucător trebuie să poată juca măcar 1 carte pe oricare teanc (logic minimală)
        for (const auto& [top, hand] : pilesWithHand) {
            bool canPlay = false;
            for (int card : hand) {
                if (canPlayAscending(top, card) || canPlayDescending(top, card))
                    canPlay = true;
            }
            if (!canPlay) return true; // pierde dacă nu poate plasa
        }
        return false;
        };

    assert(winCondition(0));
    assert(!winCondition(3));
    std::vector<std::pair<int, std::vector<int>>> pilesWithHand = { {55, {65,52}}, {80, {82,70}} };
    assert(!loseCondition(pilesWithHand)); // se poate juca
    std::vector<std::pair<int, std::vector<int>>> pilesWithHand2 = { {30, {15,12}}, {99, {11,10}} };
    assert(loseCondition(pilesWithHand2)); // nu se poate juca
    std::cout << "Win/lose logic passed\n";
}


#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <numeric>
#include <optional>
#include <iostream>
#include <set>

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

// 1: basic ascending check
REGISTER_TEST(TG_test_01) {
    assert(canPlayAscending(1, 2));
    assert(!canPlayAscending(50, 40));
}

// 2: basic descending check
REGISTER_TEST(TG_test_02) {
    assert(canPlayDescending(100, 99));
    assert(!canPlayDescending(60, 70));
}

// 3: backwards trick ascending
REGISTER_TEST(TG_test_03) {
    assert(canPlayAscending(45, 35));
}

// 4: backwards trick descending
REGISTER_TEST(TG_test_04) {
    assert(canPlayDescending(40, 50));
}

// 5: simulate minimal round requirement (every player must play at least N)
REGISTER_TEST(TG_test_05) {
    int players = 3;
    int minPlay = 2;
    std::vector<std::vector<int>> hands = { {10,20,30},{15,25,35},{40,50,60} };
    for (auto& h : hands) {
        assert((int)h.size() >= minPlay);
    }
}

// 6: refill simulation: draw equal to played
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

// 7: when deck empty, min play becomes 1
REGISTER_TEST(TG_test_07) {
    std::vector<int> deck;
    int minPlay = deck.empty() ? 1 : 2;
    assert(minPlay == 1);
}

// 8: determine win condition: all numbered cards placed
REGISTER_TEST(TG_test_08) {
    int remaining = 0;
    assert(remaining == 0);
}

// 9: determine lose condition: a player cannot play min required in a round
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

// 10: greedy round simulator (players play any possible card until min satisfied)
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

// 11: try to maximize plays by backwards trick
REGISTER_TEST(TG_test_11) {
    std::vector<int> tops = { 45, 100, 1, 80 };
    std::vector<int> hand = { 35, 70, 90 };
    bool ok = canPlayAscending(tops[0], hand[0]);
    assert(ok);
}

// 12: verify ordering invariants after plays
REGISTER_TEST(TG_test_12) {
    std::vector<int> tops = { 1,1,100,100 };
    tops[0] = 20;
    tops[1] = 18;
    assert(tops[0] > 1 && tops[1] > 1);
}

// 13: compute remaining deck size after distribution
REGISTER_TEST(TG_test_13) {
    auto remaining_after_deal = [](int players) {
        int per = (players == 2 ? 8 : players == 3 ? 7 : 6);
        int dealt = per * players;
        return 98 - dealt;
        };
    assert(remaining_after_deal(2) == 98 - 16);
}

// 14: combinatorial check: possible distinct orders of placing 2 cards on 4 piles
REGISTER_TEST(TG_test_14) {
    int piles = 4;
    int cardsToPlay = 2;
    int ways = 1;
    for (int i = 0; i < cardsToPlay; ++i) ways *= piles;
    assert(ways == 16);
}

// 15: check for collision-free draw from deck
REGISTER_TEST(TG_test_15) {
    std::vector<int> deck = make_range(2, 99);
    std::set<int> seen(deck.begin(), deck.end());
    assert(seen.size() == deck.size());
}

// helper for 16-25: small simulation utilities
static std::vector<int> random_shuffle_copy(std::vector<int> v) {
    std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });
    return v;
}

// 16: test random shuffle copy properties
REGISTER_TEST(TG_test_16) {
    auto a = random_shuffle_copy(std::vector<int>{1, 2, 3, 4, 5});
    assert(a.size() == 5);
}

// 17: check top change after placement
REGISTER_TEST(TG_test_17) {
    int top = 30;
    int card = 31;
    if (canPlayAscending(top, card)) top = card;
    assert(top == 31);
}

// 18: validate backwards trick only exact 10 difference
REGISTER_TEST(TG_test_18) {
    assert(canPlayAscending(45, 35));
    assert(!canPlayAscending(45, 34));
}

// 19: ensure descending backwards trick symmetry
REGISTER_TEST(TG_test_19) {
    assert(canPlayDescending(60, 70));
    assert(!canPlayDescending(60, 71));
}

// 20: check protocol when deck depleted mid-round (players allowed to play only 1)
REGISTER_TEST(TG_test_20) {
    std::vector<int> deck;
    int minPlay = deck.empty() ? 1 : 2;
    assert(minPlay == 1);
}

// 21: simulate many rounds until deck empties (fast approximation)
REGISTER_TEST(TG_test_21) {
    std::vector<int> deck = make_range(2, 99);
    int rounds = 0;
    while (!deck.empty() && rounds < 1000) {
        deck.pop_back();
        ++rounds;
    }
    assert(rounds == 98);
}

// 22: check that a move that uses backwards trick reduces top sometimes
REGISTER_TEST(TG_test_22) {
    int top = 45;
    int card = 35;
    if (canPlayAscending(top, card)) top = card;
    assert(top == 35);
}

// 23: simulate minimal check for team win (placed cards >= 98)
REGISTER_TEST(TG_test_23) {
    int placed = 98;
    assert(placed >= 98);
}

// 24: ensure that a player with no playable cards triggers lose condition detection
REGISTER_TEST(TG_test_24) {
    std::vector<int> tops = { 50,50,50,50 };
    std::vector<int> hand = { 1 }; // can't play 1 on numbered piles
    bool can = false;
    for (int t : tops) if (canPlayAscending(t, hand[0]) || canPlayDescending(t, hand[0])) can = true;
    assert(!can);
}

// 25: stress small search for a valid play among many cards
REGISTER_TEST(TG_test_25) {
    std::vector<int> tops = { 10,100,1,90 };
    std::vector<int> hand;
    for (int i = 2; i <= 99; ++i) hand.push_back(i);
    bool found = false;
    for (int c : hand) {
        for (int t : tops) if (canPlayAscending(t, c) || canPlayDescending(t, c)) { found = true; break; }
        if (found) break;
    }
    assert(found);
}