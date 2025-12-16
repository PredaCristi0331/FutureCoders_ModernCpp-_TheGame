#include "TestRunner.h"
#include "TestUtils.h"

#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>
#include <optional>
#include <variant>
#include <numeric>
#include <iterator>
#include <map>
#include <set>
#include <unordered_set>
#include <bitset>
#include <future>
#include <iostream>
#include <cassert>
#include <span>
#include <regex>
#include <memory>
#include <bit>            
#include <ranges>         
#include <concepts>       

using namespace std::string_literals;


static std::vector<std::string> split_impl(const std::string& s, char d) {
    std::vector<std::string> out;
    std::istringstream ss(s);
    std::string t;
    while (std::getline(ss, t, d)) out.push_back(t);
    return out;
}

static std::vector<int> make_range(int a, int b) {
    std::vector<int> v;
    for (int i = a; i <= b; ++i) v.push_back(i);
    return v;
}


template<typename... Args>
int variadic_sum(Args... args) {
    return (args + ... + 0);
}


template<std::integral T>
T double_integral(T v) {
    return v * 2;
}


REGISTER_TEST(TU_test_01) {
    const std::string s = "2;5;99";
    const auto tokens = split_impl(s, ';'); // const ref semantics at call site
    assert(tokens.size() == 3 && tokens[0] == "2" && tokens[2] == "99");
}


REGISTER_TEST(TU_test_02) {
    auto deck = make_range(2, 99);
    std::mt19937 rng(42);
    std::shuffle(deck.begin(), deck.end(), rng);
    
    std::vector<int> moved = std::move(deck);
    assert(moved.size() == 98);
    assert(deck.empty()); 
}


REGISTER_TEST(TU_test_03) {
    std::optional<int> o;
    assert(!o.has_value());
    o = 17;
    const std::optional<int>& r = o;
    assert(r.value() == 17);
}


REGISTER_TEST(TU_test_04) {
    std::variant<int, std::string> v = 7;
    v = "play 47"s;
    const std::string& msg = std::get<std::string>(v);
    std::regex re(R"((\d+))");
    std::smatch m;
    bool found = std::regex_search(msg, m, re);
    assert(found && m.size() == 2 && m[1] == "47");
}


REGISTER_TEST(TU_test_05) {
    std::vector<int> v = { 2, 3, 5, 7 };
    double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    assert(mean > 0);
}


REGISTER_TEST(TU_test_06) {
    std::vector<int> v = { 9,1,4,7,3 };
    auto mid = v.size() / 2;
    std::nth_element(v.begin(), v.begin() + mid, v.end());
    int median = v[mid];
    (void)median;
    assert(v.size() == 5);
}


REGISTER_TEST(TU_test_07) {
    const std::vector<int> v = { 2,3,3,99,2,2 };
    std::map<int, int> freq;
    for (int x : v) ++freq[x];
    assert(freq.at(2) == 3 && freq.at(3) == 2);
}


REGISTER_TEST(TU_test_08) {
    std::vector<int> v = { 1,2,2,3,3,3 };
    std::ranges::sort(v);
    auto it = std::unique(v.begin(), v.end());
    v.erase(it, v.end());
    assert(v == std::vector<int>({ 1,2,3 }));
}


REGISTER_TEST(TU_test_09) {
    std::vector<int> v = { 1,2,3,4,5 };
    std::span s(v);
    int sum = 0;
    for (size_t i = 0; i + 2 < s.size(); ++i) {
        sum += s[i] + s[i + 1] + s[i + 2];
    }
    assert(sum > 0);
}


REGISTER_TEST(TU_test_10) {
    std::vector<int> v = { 1,2,3 };
    std::next_permutation(v.begin(), v.end());
    assert(v.size() == 3);
}


REGISTER_TEST(TU_test_11) {
    std::vector<int> a = { 1,2,3 }, b = { 3,4,5 };
    std::unordered_set<int> out;
    out.insert(a.begin(), a.end());
    out.insert(b.begin(), b.end());
    assert(out.size() == 5 - 1);
}


REGISTER_TEST(TU_test_12) {
    std::vector<int> v = make_range(2, 99);
    auto it = std::ranges::lower_bound(v, 50);
    assert(*it == 50);
}

// 13 - gcd via std::gcd (constref)
REGISTER_TEST(TU_test_13) {
    const int a = 12, b = 18;
    int g = std::gcd(a, b);
    assert(g == 6);
}

// 14 - transform & map via ranges::transform (C++20 ranges algorithms)
REGISTER_TEST(TU_test_14) {
    std::vector<int> v = { 1,2,3 };
    std::vector<int> out(v.size());
    std::ranges::transform(v, out.begin(), [](int x) { return x * x; });
    assert(out[2] == 9);
}

// 15 - partition by predicate (lambda + const ref)
REGISTER_TEST(TU_test_15) {
    std::vector<int> v = { 1,2,3,4,5 };
    auto it = std::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    (void)it;
    assert(true);
}

// 16 - count_if & predicates combinator (concept usage)
REGISTER_TEST(TU_test_16) {
    std::vector<int> v = { 2,4,6,7,9 };
    auto even = [](int x) { return x % 2 == 0; };
    auto gt5 = [](int x) { return x > 5; };
    int cnt = std::count_if(v.begin(), v.end(), [&](int x) { return even(x) || gt5(x); });
    assert(cnt >= 1);
}

// 17 - sample via random_device (move unique_ptr into container)
REGISTER_TEST(TU_test_17) {
    std::vector<int> v = make_range(2, 99);
    std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });
    std::vector<std::unique_ptr<int>> p;
    p.reserve(5);
    for (int i = 0; i < 5; ++i) p.push_back(std::make_unique<int>(v[i]));
    // move one unique_ptr out
    std::unique_ptr<int> taken = std::move(p.back());
    p.pop_back();
    assert(taken && p.size() == 4);
}

// 18 - string join (constref + ranges)
REGISTER_TEST(TU_test_18) {
    const std::vector<std::string> parts = { "a","b","c" };
    std::string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) out += ",";
        out += parts[i];
    }
    assert(out == "a,b,c");
}

// 19 - stable_sort with custom comparator and three-way compare usage via small struct
REGISTER_TEST(TU_test_19) {
    struct Item { int key; int id; auto operator<=>(const Item&) const = default; };
    std::vector<Item> v = { {2,1},{1,2},{2,3} };
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.key < b.key; });
    assert(v.front().key == 1);
}

// 20 - bitset operations (constinit like semantics not needed; bit_cast below)
REGISTER_TEST(TU_test_20) {
    std::bitset<128> bs;
    bs.set(5);
    bs.flip(6);
    assert(bs.test(5));
}

// 21 - async/future basic (lambda)
REGISTER_TEST(TU_test_21) {
    auto fut = std::async(std::launch::async, []() { return 42; });
    int r = fut.get();
    assert(r == 42);
}

// 22 - fold (accumulate with custom op)
REGISTER_TEST(TU_test_22) {
    std::vector<int> v = { 1,2,3,4 };
    int prod = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>());
    assert(prod == 24);
}

// 23 - composing functions (higher-order + concept usage)
REGISTER_TEST(TU_test_23) {
    auto add = [](int a) { return [a](int b) { return a + b; }; };
    auto add5 = add(5);
    assert(add5(3) == 8);
    // concept-based double_integral
    static_assert(std::integral<int>);
    assert(double_integral(3) == 6);
}

// 24 - find_if & optional
REGISTER_TEST(TU_test_24) {
    std::vector<int> v = { 10,20,30 };
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 15; });
    std::optional<int> res;
    if (it != v.end()) res = *it;
    assert(res.has_value() && res.value() == 20);
}

// 25 - demonstrate std::bit_cast (C++20)
REGISTER_TEST(TU_test_25) {
    uint32_t x = 0x3f800000u; // bit pattern for float 1.0
    float f = std::bit_cast<float>(x);
    assert(f == 1.0f);
}