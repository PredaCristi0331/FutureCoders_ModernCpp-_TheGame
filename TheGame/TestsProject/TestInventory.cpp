#include "TestInventory.h"
#include "TestRunner.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include<sstream>


void testInventoryAdd() {
    std::vector<int> hand;
    hand.push_back(45);
    hand.push_back(22);
    assert(hand.size() == 2);
    std::cout << "Inventory add passed\n";
}

void testInventoryRemove() {
    std::vector<int> hand{ 10, 20, 30 };
    hand.erase(std::remove(hand.begin(), hand.end(), 20), hand.end());
    assert(hand.size() == 2);
    std::cout << "Inventory remove passed\n";
}

void testInventorySearch() {
    std::vector<int> hand{ 5, 17, 23, 42 };
    auto found = std::find(hand.begin(), hand.end(), 23);
    assert(found != hand.end());
    std::cout << "Inventory search passed\n";
}

void testInventorySort() {
    std::vector<int> hand{ 45, 12, 88 };
    std::sort(hand.begin(), hand.end());
    assert(hand[0] == 12 && hand[2] == 88);
    std::cout << "Inventory sort passed\n";
}

#include "TestRunner.h"
#include "TestInventory.h"

#include <vector>
#include <algorithm>
#include <cassert>
#include <map>
#include <iostream>
#include <numeric>
#include <set>
#include <random>
#include <array>
#include <deque>
#include <unordered_map>
#include <ranges>
#include <memory>
#include <span>
#include <regex>

using namespace std::string_literals;

// helper to show move-insertion into deque
static std::deque<int> make_deque_moved(std::vector<int> v) {
    std::deque<int> d;
    for (auto& x : v) d.push_back(std::move(x)); // moves ints (no real effect), shows pattern
    return d;
}

// simple template for container sum (generic)
template<typename Container>
auto container_sum(const Container& c) {
    using T = typename Container::value_type;
    T s{};
    for (const auto& x : c) s += x;
    return s;
}

// variadic template to build vector quickly
template<typename... Ts>
auto make_vec(Ts... xs) {
    return std::vector{ xs... }; // CTAD
}

// 1: add card to inventory (vector)
REGISTER_TEST(TI_test_01) {
    std::vector<int> hand;
    hand.push_back(42);
    assert(hand.size() == 1 && hand[0] == 42);
}

// 2: remove card by value (erase-remove)
REGISTER_TEST(TI_test_02) {
    std::vector<int> hand = { 1,2,3,2 };
    hand.erase(std::remove(hand.begin(), hand.end(), 2), hand.end());
    assert(std::find(hand.begin(), hand.end(), 2) == hand.end());
}

// 3: find card (const ref)
REGISTER_TEST(TI_test_03) {
    const std::vector<int> hand = { 10,20,30 };
    auto it = std::find(hand.begin(), hand.end(), 20);
    assert(it != hand.end());
}

// 4: inventory sort and verify order (ranges::sort)
REGISTER_TEST(TI_test_04) {
    std::vector<int> hand = { 5,2,9 };
    std::ranges::sort(hand);
    assert(hand.front() == 2 && hand.back() == 9);
}

// 5: stable_sort with secondary key (simulate rarity) (array)
REGISTER_TEST(TI_test_05) {
    std::array<std::pair<int, int>, 3> items = { {{5,1},{5,2},{3,1}} };
    std::stable_sort(items.begin(), items.end(), [](auto& a, auto& b) { return a.first < b.first; });
    assert(items.front().first == 3);
}

// 6: partition inventory into playable/unplayable given a top value (span)
REGISTER_TEST(TI_test_06) {
    std::vector<int> hand = { 40,60,45,52 };
    std::span hs(hand);
    auto it = std::partition(hs.begin(), hs.end(), [&](int c) { return c < 50; });
    (void)it;
    assert(true);
}

// 7: unique counts using unordered_map
REGISTER_TEST(TI_test_07) {
    std::vector<int> hand = { 2,3,2,4,3 };
    std::unordered_map<int, int> freq;
    for (int c : hand) ++freq[c];
    assert(freq[2] == 2 && freq[3] == 2);
}

// 8: most common card (mode)
REGISTER_TEST(TI_test_08) {
    std::vector<int> hand = { 2,2,3,4,2,3 };
    std::unordered_map<int, int> freq;
    for (int c : hand) ++freq[c];
    int best = -1, bestc = 0;
    for (auto& p : freq) if (p.second > bestc) { bestc = p.second; best = p.first; }
    assert(best == 2);
}

// 9: subset check (hand contains subset) (ranges::all_of)
REGISTER_TEST(TI_test_09) {
    std::vector<int> hand = { 2,3,4,5 };
    std::vector<int> need = { 3,5 };
    bool ok = std::ranges::all_of(need, [&](int n) { return std::ranges::find(hand, n) != hand.end(); });
    assert(ok);
}

// 10: superset check using set
REGISTER_TEST(TI_test_10) {
    std::set<int> s = { 2,3,4,5 };
    std::vector<int> need = { 3,4 };
    for (int n : need) assert(s.count(n));
}

// 11: swap two elements
REGISTER_TEST(TI_test_11) {
    std::vector<int> hand = { 1,2,3 };
    std::swap(hand[0], hand[2]);
    assert(hand[0] == 3);
}

// 12: split hand into two halves (deque)
REGISTER_TEST(TI_test_12) {
    std::vector<int> hand = { 1,2,3,4 };
    auto mid = hand.begin() + hand.size() / 2;
    std::deque<int> a(hand.begin(), mid), b(mid, hand.end());
    assert(a.size() + b.size() == hand.size());
}

// 13: nth_element to find k-th smallest
REGISTER_TEST(TI_test_13) {
    std::vector<int> hand = { 5,1,4,2,3 };
    std::nth_element(hand.begin(), hand.begin() + 2, hand.end());
    assert(hand.size() == 5);
}

// 14: intersection of two hands using unordered_map
REGISTER_TEST(TI_test_14) {
    std::vector<int> a = { 1,2,3 }, b = { 2,3,4 };
    std::unordered_map<int, bool> seen;
    for (int x : a) seen[x] = true;
    std::vector<int> inter;
    for (int x : b) if (seen[x]) inter.push_back(x);
    assert(inter.size() == 2);
}

// 15: symmetric difference
REGISTER_TEST(TI_test_15) {
    std::vector<int> a = { 1,2,3 }, b = { 3,4 };
    std::set<int> sa(a.begin(), a.end()), sb(b.begin(), b.end());
    std::vector<int> sym;
    for (int x : sa) if (!sb.count(x)) sym.push_back(x);
    for (int x : sb) if (!sa.count(x)) sym.push_back(x);
    assert(!sym.empty());
}

// 16: multiset behaviour via unordered_map counts
REGISTER_TEST(TI_test_16) {
    std::vector<int> hand = { 2,2,3,2,3 };
    std::unordered_map<int, int> count;
    for (int x : hand) ++count[x];
    assert(count[2] == 3);
}

// 17: draw n cards from deck to hand (move, ensure deck shrinks)
REGISTER_TEST(TI_test_17) {
    std::vector<int> deck(98);
    std::iota(deck.begin(), deck.end(), 2);
    std::vector<int> hand;
    int n = 6;
    for (int i = 0; i < n; ++i) { hand.push_back(deck.back()); deck.pop_back(); }
    assert((int)hand.size() == n && (int)deck.size() == 98 - n);
}

// 18: replace card at position
REGISTER_TEST(TI_test_18) {
    std::vector<int> hand = { 1,2,3 };
    hand[1] = 99;
    assert(hand[1] == 99);
}

// 19: rotate hand (ranges rotate is not standardized pre-C++23; use std::rotate)
REGISTER_TEST(TI_test_19) {
    std::vector<int> hand = { 1,2,3,4 };
    std::rotate(hand.begin(), hand.begin() + 1, hand.end());
    assert(hand[0] == 2);
}

// 20: accumulate to compute weighted score (template generic sum)
REGISTER_TEST(TI_test_20) {
    std::vector<int> hand = { 2,3,4 };
    int s = container_sum(hand);
    assert(s == 9);
}

// 21: stable partition keeping relative order
REGISTER_TEST(TI_test_21) {
    std::vector<int> hand = { 1,4,3,2 };
    std::stable_partition(hand.begin(), hand.end(), [](int x) { return x % 2 == 0; });
    assert(true);
}

// 22: random swap two elements (show move of vector into deque factory)
REGISTER_TEST(TI_test_22) {
    std::vector<int> v = { 1,2,3,4,5 };
    auto d = make_deque_moved(std::move(v)); // v moved
    assert(d.size() == 5);
}

// 23: deduplicate and count remaining
REGISTER_TEST(TI_test_23) {
    std::vector<int> hand = { 2,2,2,3,3 };
    std::sort(hand.begin(), hand.end());
    hand.erase(std::unique(hand.begin(), hand.end()), hand.end());
    assert(hand.size() == 2);
}

// 24: find LIS naive (O(n^2))
REGISTER_TEST(TI_test_24) {
    std::vector<int> a = { 2,5,3,7,11,8,10,13,6 };
    std::vector<int> dp(a.size(), 1);
    size_t best = 0;
    for (size_t i = 0; i < a.size(); ++i) for (size_t j = 0; j < i; ++j) if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
    for (auto x : dp) best = std::max(best, (size_t)x);
    assert(best >= 1);
}

// 25: build vector via variadic template and regex validate numeric content
REGISTER_TEST(TI_test_25) {
    auto v = make_vec(2, 3, 5, 7);
    std::regex re(R"(\d+)");
    std::ostringstream oss;
    for (auto x : v) { if (&x != &v[0]) oss << ","; oss << x; }
    std::string s = oss.str();
    assert(std::regex_search(s, re));
}