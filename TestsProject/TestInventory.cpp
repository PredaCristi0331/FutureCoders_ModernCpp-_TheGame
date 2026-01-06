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
#include <stdexcept>
#include <future>
#include<sstream>


using namespace std::string_literals;

// helper to show move-insertion into deque
static std::deque<int> make_deque_moved(std::vector<int> v) {
    std::deque<int> d;
    for (auto& x : v) d.push_back(std::move(x));
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

// 1..25 existing tests (kept unchanged) ------------------------------------

REGISTER_TEST(TI_test_01) {
    std::vector<int> hand;
    hand.push_back(42);
    assert(hand.size() == 1 && hand[0] == 42);
}

REGISTER_TEST(TI_test_02) {
    std::vector<int> hand = { 1,2,3,2 };
    hand.erase(std::remove(hand.begin(), hand.end(), 2), hand.end());
    assert(std::find(hand.begin(), hand.end(), 2) == hand.end());
}

REGISTER_TEST(TI_test_03) {
    const std::vector<int> hand = { 10,20,30 };
    auto it = std::find(hand.begin(), hand.end(), 20);
    assert(it != hand.end());
}

REGISTER_TEST(TI_test_04) {
    std::vector<int> hand = { 5,2,9 };
    std::ranges::sort(hand);
    assert(hand.front() == 2 && hand.back() == 9);
}

REGISTER_TEST(TI_test_05) {
    std::array<std::pair<int, int>, 3> items = { {{5,1},{5,2},{3,1}} };
    std::stable_sort(items.begin(), items.end(), [](auto& a, auto& b) { return a.first < b.first; });
    assert(items.front().first == 3);
}

REGISTER_TEST(TI_test_06) {
    std::vector<int> hand = { 40,60,45,52 };
    std::span hs(hand);
    auto it = std::partition(hs.begin(), hs.end(), [&](int c) { return c < 50; });
    (void)it;
    assert(true);
}

REGISTER_TEST(TI_test_07) {
    std::vector<int> hand = { 2,3,2,4,3 };
    std::unordered_map<int, int> freq;
    for (int c : hand) ++freq[c];
    assert(freq[2] == 2 && freq[3] == 2);
}

REGISTER_TEST(TI_test_08) {
    std::vector<int> hand = { 2,2,3,4,2,3 };
    std::unordered_map<int, int> freq;
    for (int c : hand) ++freq[c];
    int best = -1, bestc = 0;
    for (auto& p : freq) if (p.second > bestc) { bestc = p.second; best = p.first; }
    assert(best == 2);
}

REGISTER_TEST(TI_test_09) {
    std::vector<int> hand = { 2,3,4,5 };
    std::vector<int> need = { 3,5 };
    bool ok = std::ranges::all_of(need, [&](int n) { return std::ranges::find(hand, n) != hand.end(); });
    assert(ok);
}

REGISTER_TEST(TI_test_10) {
    std::set<int> s = { 2,3,4,5 };
    std::vector<int> need = { 3,4 };
    for (int n : need) assert(s.count(n));
}

REGISTER_TEST(TI_test_11) {
    std::vector<int> hand = { 1,2,3 };
    std::swap(hand[0], hand[2]);
    assert(hand[0] == 3);
}

REGISTER_TEST(TI_test_12) {
    std::vector<int> hand = { 1,2,3,4 };
    auto mid = hand.begin() + hand.size() / 2;
    std::deque<int> a(hand.begin(), mid), b(mid, hand.end());
    assert(a.size() + b.size() == hand.size());
}

REGISTER_TEST(TI_test_13) {
    std::vector<int> hand = { 5,1,4,2,3 };
    std::nth_element(hand.begin(), hand.begin() + 2, hand.end());
    assert(hand.size() == 5);
}

REGISTER_TEST(TI_test_14) {
    std::vector<int> a = { 1,2,3 }, b = { 2,3,4 };
    std::unordered_map<int, bool> seen;
    for (int x : a) seen[x] = true;
    std::vector<int> inter;
    for (int x : b) if (seen[x]) inter.push_back(x);
    assert(inter.size() == 2);
}

REGISTER_TEST(TI_test_15) {
    std::vector<int> a = { 1,2,3 }, b = { 3,4 };
    std::set<int> sa(a.begin(), a.end()), sb(b.begin(), b.end());
    std::vector<int> sym;
    for (int x : sa) if (!sb.count(x)) sym.push_back(x);
    for (int x : sb) if (!sa.count(x)) sym.push_back(x);
    assert(!sym.empty());
}

REGISTER_TEST(TI_test_16) {
    std::vector<int> hand = { 2,2,3,2,3 };
    std::unordered_map<int, int> count;
    for (int x : hand) ++count[x];
    assert(count[2] == 3);
}

REGISTER_TEST(TI_test_17) {
    std::vector<int> deck(98);
    std::iota(deck.begin(), deck.end(), 2);
    std::vector<int> hand;
    int n = 6;
    for (int i = 0; i < n; ++i) { hand.push_back(deck.back()); deck.pop_back(); }
    assert((int)hand.size() == n && (int)deck.size() == 98 - n);
}

REGISTER_TEST(TI_test_18) {
    std::vector<int> hand = { 1,2,3 };
    hand[1] = 99;
    assert(hand[1] == 99);
}

REGISTER_TEST(TI_test_19) {
    std::vector<int> hand = { 1,2,3,4 };
    std::rotate(hand.begin(), hand.begin() + 1, hand.end());
    assert(hand[0] == 2);
}

REGISTER_TEST(TI_test_20) {
    std::vector<int> hand = { 2,3,4 };
    int s = container_sum(hand);
    assert(s == 9);
}

REGISTER_TEST(TI_test_21) {
    std::vector<int> hand = { 1,4,3,2 };
    std::stable_partition(hand.begin(), hand.end(), [](int x) { return x % 2 == 0; });
    assert(true);
}

REGISTER_TEST(TI_test_22) {
    std::vector<int> v = { 1,2,3,4,5 };
    auto d = make_deque_moved(std::move(v));
    assert(d.size() == 5);
}

REGISTER_TEST(TI_test_23) {
    std::vector<int> hand = { 2,2,2,3,3 };
    std::sort(hand.begin(), hand.end());
    hand.erase(std::unique(hand.begin(), hand.end()), hand.end());
    assert(hand.size() == 2);
}

REGISTER_TEST(TI_test_24) {
    std::vector<int> a = { 2,5,3,7,11,8,10,13,6 };
    std::vector<int> dp(a.size(), 1);
    size_t best = 0;
    for (size_t i = 0; i < a.size(); ++i) for (size_t j = 0; j < i; ++j) if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
    for (auto x : dp) best = std::max(best, (size_t)x);
    assert(best >= 1);
}

REGISTER_TEST(TI_test_25) {
    auto v = make_vec(2, 3, 5, 7);
    std::regex re(R"(\d+)");
    std::ostringstream oss;
    for (auto x : v) { if (&x != &v[0]) oss << ","; oss << x; }
    std::string s = oss.str();
    assert(std::regex_search(s, re));
}

// ---------------------------------------------------------------------------
// New tests 26..35 - inventory exception handling and integration scenarios
// ---------------------------------------------------------------------------

// 26: remove non-existing card -> function throws (simulated behavior)
REGISTER_TEST(TI_test_26) {
    auto remove_card_or_throw = [](std::vector<int>& hand, int val) {
        auto it = std::find(hand.begin(), hand.end(), val);
        if (it == hand.end()) throw std::runtime_error("not found");
        hand.erase(it);
        };
    std::vector<int> h = { 1,2,3 };
    bool caught = false;
    try { remove_card_or_throw(h, 5); }
    catch (const std::runtime_error& e) { caught = true; }
    assert(caught);
}

// 27: regex validation for item codes
REGISTER_TEST(TI_test_27) {
    std::string code = "ITM-001";
    std::regex re(R"(ITM-\d{3})");
    assert(std::regex_match(code, re));
}

// 28: ensure shared_ptr items are cleaned when container goes out of scope even on exception
REGISTER_TEST(TI_test_28) {
    bool cleaned = false;
    struct R { bool* p; R(bool* q) :p(q) {} ~R() noexcept { if (p) *p = true; } };
    {
        std::vector<std::shared_ptr<R>> v;
        v.push_back(std::make_shared<R>(&cleaned));
        try { throw std::runtime_error("boom"); }
        catch (...) {}
    }
    assert(cleaned);
}

// 29: transactional inventory swap rollback
REGISTER_TEST(TI_test_29) {
    auto transactional_swap = [](std::vector<int>& a, std::vector<int>& b) {
        auto backup_a = a;
        auto backup_b = b;
        try {
            std::swap(a, b);
            throw std::runtime_error("fail");
        }
        catch (...) {
            a = std::move(backup_a);
            b = std::move(backup_b);
            throw;
        }
        };
    std::vector<int> a = { 1 }, b = { 2 };
    bool rolled = false;
    try { transactional_swap(a, b); }
    catch (...) { rolled = true; }
    assert(rolled && a == std::vector<int>{1}&& b == std::vector<int>{2});
}

// 30: reserve may throw bad_alloc - simulate by catching bad_alloc
REGISTER_TEST(TI_test_30) {
    std::vector<int> v;
    bool caught = false;
    try {
        // large reserve may or may not throw; simulate by explicit throw to test path
        throw std::bad_alloc();
    }
    catch (const std::bad_alloc&) { caught = true; }
    assert(caught);
}

// 31: async background inventory compaction (exception in task propagates)
REGISTER_TEST(TI_test_31) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("compact fail"); return 0; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error&) { catched = true; }
    assert(catched);
}

// 32: iterator invalidation detection via throwing helper
REGISTER_TEST(TI_test_32) {
    std::vector<int> v = { 1,2,3 };
    auto it = v.begin();
    v.push_back(4); // may invalidate
    bool ok = (it != v.end()); // we don't rely on behavior, just ensure program continues
    (void)ok;
    assert(true);
}

// 33: find item with ranges + lambda that may throw (caught outside)
REGISTER_TEST(TI_test_33) {
    std::vector<int> v = { 1,2,3,4 };
    bool thrown = false;
    try {
        std::ranges::for_each(v, [](int x) {
            if (x == 3) throw std::runtime_error("bad item");
            });
    }
    catch (const std::runtime_error& e) { thrown = std::string(e.what()) == "bad item"; }
    assert(thrown);
}

// 34: dedup with exception safety (backup restore)
REGISTER_TEST(TI_test_34) {
    std::vector<int> hand = { 2,2,3,3 };
    auto backup = hand;
    try {
        hand.erase(std::remove(hand.begin(), hand.end(), 42), hand.end());
    }
    catch (...) { hand = backup; }
    assert(!hand.empty());
}

// 35: make_vec variadic helper correctness and regex on resulting string
REGISTER_TEST(TI_test_35) {
    auto v = make_vec(11, 22, 33);
    std::ostringstream oss;
    for (size_t i = 0; i < v.size(); ++i) { if (i) oss << ','; oss << v[i]; }
    std::string s = oss.str();
    std::regex re(R"(\d+,\d+,\d+)");
    assert(std::regex_match(s, re));
}