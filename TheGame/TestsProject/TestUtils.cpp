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
#include <bitset>
#include <future>
#include <iostream>
#include <cassert>
#include <span>
#include <regex>
#include <memory>
#include <bit>            // std::bit_cast
#include <ranges>         // ranges library
#include <concepts>       // concepts for template constraints
#include <stdexcept>
#include <thread>
#include <unordered_set>

using namespace std::string_literals;

// constinit example (C++20)
constinit static int g_constinit_counter = 7;

// Generic helpers (local to this translation unit)
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

// Variadic template helper (demonstrates fold expressions)
template<typename... Args>
int variadic_sum(Args... args) {
    return (args + ... + 0);
}

// Template constrained by concept (C++20)
template<std::integral T>
T double_integral(T v) {
    return v * 2;
}

// Small consteval function (C++20)
consteval int consteval_square(int x) { return x * x; }

// 1 - 25 (existing tests) ---------------------------------------------------
// (kept same as previously defined tests 01..25, omitted here for brevity in explanation)
// We include them as they were. For brevity in the message we still provide full content:
// 1
REGISTER_TEST(TU_test_01) {
    const std::string s = "2;5;99";
    const auto tokens = split_impl(s, ';'); // const ref semantics at call site
    assert(tokens.size() == 3 && tokens[0] == "2" && tokens[2] == "99");
}

// 2: shuffle reproducible with seed (move semantics demonstration)
REGISTER_TEST(TU_test_02) {
    auto deck = make_range(2, 99);
    std::mt19937 rng(42);
    std::shuffle(deck.begin(), deck.end(), rng);
    std::vector<int> moved = std::move(deck);
    assert(moved.size() == 98);
    assert(deck.empty()); // moved-from should be empty
}

// 3 - 25 (the rest of previously provided tests)
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

REGISTER_TEST(TU_test_13) {
    const int a = 12, b = 18;
    int g = std::gcd(a, b);
    assert(g == 6);
}

REGISTER_TEST(TU_test_14) {
    std::vector<int> v = { 1,2,3 };
    std::vector<int> out(v.size());
    std::ranges::transform(v, out.begin(), [](int x) { return x * x; });
    assert(out[2] == 9);
}

REGISTER_TEST(TU_test_15) {
    std::vector<int> v = { 1,2,3,4,5 };
    auto it = std::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    (void)it;
    assert(true);
}

REGISTER_TEST(TU_test_16) {
    std::vector<int> v = { 2,4,6,7,9 };
    auto even = [](int x) { return x % 2 == 0; };
    auto gt5 = [](int x) { return x > 5; };
    int cnt = std::count_if(v.begin(), v.end(), [&](int x) { return even(x) || gt5(x); });
    assert(cnt >= 1);
}

REGISTER_TEST(TU_test_17) {
    std::vector<int> v = make_range(2, 99);
    std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });
    std::vector<std::unique_ptr<int>> p;
    p.reserve(5);
    for (int i = 0; i < 5; ++i) p.push_back(std::make_unique<int>(v[i]));
    std::unique_ptr<int> taken = std::move(p.back());
    p.pop_back();
    assert(taken && p.size() == 4);
}

REGISTER_TEST(TU_test_18) {
    const std::vector<std::string> parts = { "a","b","c" };
    std::string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) out += ",";
        out += parts[i];
    }
    assert(out == "a,b,c");
}

REGISTER_TEST(TU_test_19) {
    struct Item { int key; int id; auto operator<=>(const Item&) const = default; };
    std::vector<Item> v = { {2,1},{1,2},{2,3} };
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.key < b.key; });
    assert(v.front().key == 1);
}

REGISTER_TEST(TU_test_20) {
    std::bitset<128> bs;
    bs.set(5);
    bs.flip(6);
    assert(bs.test(5));
}

REGISTER_TEST(TU_test_21) {
    auto fut = std::async(std::launch::async, []() { return 42; });
    int r = fut.get();
    assert(r == 42);
}

REGISTER_TEST(TU_test_22) {
    std::vector<int> v = { 1,2,3,4 };
    int prod = std::accumulate(v.begin(), v.end(), 1, std::multiplies<>());
    assert(prod == 24);
}

REGISTER_TEST(TU_test_23) {
    auto add = [](int a) { return [a](int b) { return a + b; }; };
    auto add5 = add(5);
    assert(add5(3) == 8);
    static_assert(std::integral<int>);
    assert(double_integral(3) == 6);
}

REGISTER_TEST(TU_test_24) {
    std::vector<int> v = { 10,20,30 };
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 15; });
    std::optional<int> res;
    if (it != v.end()) res = *it;
    assert(res.has_value() && res.value() == 20);
}

REGISTER_TEST(TU_test_25) {
    uint32_t x = 0x3f800000u; // bit pattern for float 1.0
    float f = std::bit_cast<float>(x);
    assert(f == 1.0f);
}

// ---------------------------------------------------------------------------
// New tests added: 26..35 - exception handling, consteval/constinit, async, move semantics
// ---------------------------------------------------------------------------

// 26: parse-like function that throws on invalid input (exception path)
REGISTER_TEST(TU_test_26) {
    auto parseIntThrow = [](const std::string& s)->int {
        try {
            size_t idx = 0;
            long v = std::stol(s, &idx);
            if (idx != s.size()) throw std::invalid_argument("trailing");
            return static_cast<int>(v);
        }
        catch (const std::invalid_argument&) {
            throw std::runtime_error("invalid");
        }
        catch (const std::out_of_range&) {
            throw std::runtime_error("range");
        }
        };
    bool caught = false;
    try {
        parseIntThrow("12x");
    }
    catch (const std::runtime_error& e) {
        caught = true;
        assert(std::string(e.what()) == "invalid");
    }
    assert(caught);
}

// 27: std::regex_error thrown for bad pattern
REGISTER_TEST(TU_test_27) {
    bool caught = false;
    try {
        std::regex r("[");
        (void)r;
    }
    catch (const std::regex_error&) {
        caught = true;
    }
    assert(caught);
}

// 28: unique_ptr RAII cleanup on exception
REGISTER_TEST(TU_test_28) {
    bool cleaned = false;
    struct Res { bool* p; Res(bool* q) :p(q) {} ~Res() noexcept { if (p) *p = true; } };
    try {
        auto r = std::make_unique<Res>(&cleaned);
        throw std::runtime_error("boom");
    }
    catch (...) {}
    assert(cleaned);
}

// 29: consteval function correctness (compile-time function used at runtime)
REGISTER_TEST(TU_test_29) {
    constexpr int sq = consteval_square(6);
    assert(sq == 36);
}

// 30: constinit global variable present and has expected value
REGISTER_TEST(TU_test_30) {
    // g_constinit_counter defined at top
    assert(g_constinit_counter == 7);
}

// 31: std::async exception propagation
REGISTER_TEST(TU_test_31) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("async"); return 1; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error& e) { catched = (std::string(e.what()) == "async"); }
    assert(catched);
}

// 32: moving container of unique_ptr leaves source in valid moved-from state
REGISTER_TEST(TU_test_32) {
    std::vector<std::unique_ptr<int>> a;
    a.push_back(std::make_unique<int>(5));
    auto b = std::move(a);
    assert(a.empty());
    assert(!b.empty());
}

// 33: variadic_sum template correctness
REGISTER_TEST(TU_test_33) {
    assert(variadic_sum(1, 2, 3, 4) == 10);
    assert(variadic_sum() == 0);
}

// 34: transactional rollback helper (copy & swap) for vector
REGISTER_TEST(TU_test_34) {
    auto transactional_modify = [](std::vector<int>& c, auto op) {
        std::vector<int> backup = c;
        try {
            op(c);
        }
        catch (...) {
            c = std::move(backup);
            throw;
        }
        };
    std::vector<int> v = { 1,2,3 };
    try { transactional_modify(v, [](auto& x) { x.push_back(4); throw std::runtime_error("fail"); }); }
    catch (...) {}
    assert((v == std::vector<int>{1, 2, 3}));
}

// 35: ranges any_of with lambda that may throw but caught outside
REGISTER_TEST(TU_test_35) {
    std::vector<int> v = { 1,2,3,4 };
    bool threw = false;
    try {
        std::ranges::for_each(v, [](int x) {
            if (x == 3) throw std::runtime_error("found");
            });
    }
    catch (const std::runtime_error& e) {
        threw = std::string(e.what()) == "found";
    }
    assert(threw);
}