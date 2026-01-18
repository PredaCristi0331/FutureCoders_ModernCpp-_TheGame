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
#include <bit>            
#include <ranges>         
#include <concepts>       
#include <stdexcept>
#include <thread>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include<fstream>


using namespace std::string_literals;
namespace fs = std::filesystem;

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
    assert(deck.empty()); // moved-from should be empty
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

REGISTER_TEST(TU_test_29) {
    constexpr int sq = consteval_square(6);
    assert(sq == 36);
}

REGISTER_TEST(TU_test_30) {
    assert(g_constinit_counter == 7);
}

REGISTER_TEST(TU_test_31) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("async"); return 1; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error& e) { catched = (std::string(e.what()) == "async"); }
    assert(catched);
}

REGISTER_TEST(TU_test_32) {
    std::vector<std::unique_ptr<int>> a;
    a.push_back(std::make_unique<int>(5));
    auto b = std::move(a);
    assert(a.empty());
    assert(!b.empty());
}

REGISTER_TEST(TU_test_33) {
    assert(variadic_sum(1, 2, 3, 4) == 10);
    assert(variadic_sum() == 0);
}

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



static std::vector<fs::path> collect_source_files() {
    std::vector<fs::path> out;
    const fs::path start = fs::current_path();
    for (auto const& entry : fs::recursive_directory_iterator(start)) {
        if (!entry.is_regular_file()) continue;
        auto p = entry.path();
        auto s = p.string();

        if (s.find("/tests/") != std::string::npos || s.find("\\tests\\") != std::string::npos) continue;
        if (p.extension() == ".cpp" || p.extension() == ".cc" || p.extension() == ".cxx" || p.extension() == ".h" || p.extension() == ".hpp")
            out.push_back(p);
    }
    return out;
}

static std::vector<std::string> read_lines(const fs::path& p) {
    std::vector<std::string> lines;
    std::ifstream ifs(p);
    if (!ifs) return lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);
    return lines;
}

static std::string trim_copy(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

static bool is_comment_line(const std::string& s) {
    auto t = trim_copy(s);
    return t.rfind("//", 0) == 0 || t.rfind("/*", 0) == 0 || t.rfind("*", 0) == 0;
}

REGISTER_TEST(TU_test_36_duplicate_code) {
    auto files = collect_source_files();
    const int WINDOW = 5;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> seen; // fingerprint -> list of (file,line)
    for (auto& f : files) {
        auto lines = read_lines(f);
        std::vector<std::string> norm;
        norm.reserve(lines.size());
        for (auto& L : lines) {
            auto t = trim_copy(L);
            if (t.empty() || is_comment_line(t)) continue;
            norm.push_back(t);
        }
        for (size_t i = 0; i + WINDOW <= norm.size(); ++i) {
            std::ostringstream oss;
            for (int j = 0; j < WINDOW; ++j) oss << norm[i + j] << '\n';
            std::string key = oss.str();
            seen[key].emplace_back(f.string(), int(i + 1));
        }
    }

    std::vector<std::string> dup_examples;
    for (auto& kv : seen) {
        std::set<std::string> fileset;
        for (auto& loc : kv.second) fileset.insert(loc.first);
        if (fileset.size() >= 2) {

            std::ostringstream oss;
            int added = 0;
            for (auto& loc : kv.second) {
                if (added++ >= 6) break;
                oss << loc.first << ":" << loc.second << "; ";
            }
            dup_examples.push_back(oss.str());
            if (dup_examples.size() >= 5) break;
        }
    }
    if (!dup_examples.empty()) {
        std::ostringstream msg;
        msg << "Duplicate code fragments detected (heuristic): examples: ";
        for (auto& e : dup_examples) msg << "[" << e << "]";
        throw std::runtime_error(msg.str());
    }
}


REGISTER_TEST(TU_test_37_magic_numbers) {
    auto files = collect_source_files();
    std::regex number_re(R"((?<![\w.])(-?\d+)(?![\w.]))");
    std::vector<std::string> examples;
    for (auto& f : files) {
        auto lines = read_lines(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            const auto& L = lines[i];
            std::string trimmed = trim_copy(L);
            if (trimmed.empty()) continue;

            std::string low = trimmed;
            std::transform(low.begin(), low.end(), low.begin(), [](unsigned char c) { return std::tolower(c); });
            if (low.find("constexpr") != std::string::npos ||
                low.find("const ") != std::string::npos ||
                low.find("#define") != std::string::npos ||
                low.find("enum ") != std::string::npos) continue;

            std::smatch m;
            std::string s = L;
            auto begin = s.cbegin();
            while (std::regex_search(begin, s.cend(), m, number_re)) {
                std::string num = m[1].str();
                try {
                    long val = std::stol(num);

                    if (val == 0 || val == 1 || val == -1) {

                    }
                    else {

                        std::ostringstream ex;
                        ex << f.string() << ":" << (i + 1) << " -> " << trim_copy(L);
                        examples.push_back(ex.str());
                        break;
                    }
                }
                catch (...) {}
                begin = m.suffix().first;
            }
            if (examples.size() >= 10) break;
        }
        if (examples.size() >= 10) break;
    }
    if (!examples.empty()) {
        std::ostringstream msg;
        msg << "Probable magic number literals found (heuristic). Examples:\n";
        for (auto& e : examples) msg << "  " << e << "\n";
        msg << "Recommendation: replace literals with named constants (const/constexpr/enums).";
        throw std::runtime_error(msg.str());
    }
}


REGISTER_TEST(TU_test_38_long_functions) {
    auto files = collect_source_files();
    const int MAX_LINES = 50;
    std::vector<std::string> long_funcs;
    std::regex control_kw(R"(\b(if|for|while|switch|catch|else|return|do)\b)");
    for (auto& f : files) {
        auto lines = read_lines(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string L = trim_copy(lines[i]);
            if (L.empty()) continue;

            if (L.find(')') != std::string::npos && L.back() != ';') {

                if (std::regex_search(L, control_kw)) continue;

                size_t braceLine = i;
                bool foundBrace = false;
                if (L.find('{') != std::string::npos) foundBrace = true;
                else {
                    size_t j = i + 1;
                    for (; j < lines.size() && j < i + 6; ++j) {
                        auto next = trim_copy(lines[j]);
                        if (next.empty()) continue;
                        if (next.front() == '{') { braceLine = j; foundBrace = true; break; }
                        break;
                    }
                }
                if (!foundBrace) continue;

                int level = 0;
                int countLines = 0;
                for (size_t k = braceLine; k < lines.size(); ++k) {
                    auto t = lines[k];
                    for (char c : t) {
                        if (c == '{') ++level;
                        else if (c == '}') --level;
                    }
                    ++countLines;
                    if (level <= 0) break;
                    if (countLines > MAX_LINES) {
                        std::ostringstream ex;
                        ex << f.string() << ":" << (i + 1) << " header=" << L << " length=" << countLines;
                        long_funcs.push_back(ex.str());
                        break;
                    }
                }
                if (long_funcs.size() >= 10) break;
            }
        }
        if (long_funcs.size() >= 10) break;
    }
    if (!long_funcs.empty()) {
        std::ostringstream msg;
        msg << "Functions exceeding " << 50 << " lines detected (heuristic). Examples:\n";
        for (auto& e : long_funcs) msg << "  " << e << "\n";
        msg << "Consider refactoring into smaller functions.";
        throw std::runtime_error(msg.str());
    }
}


REGISTER_TEST(TU_test_39_many_params) {
    auto files = collect_source_files();
    std::vector<std::string> offenders;
    std::regex func_sig(R"(([^;{}()\n<>]+)\(([^\)]*)\)\s*(const)?\s*(?:\{|;))");
    for (auto& f : files) {
        auto text = std::string();
        for (auto& L : read_lines(f)) text += L + "\n";
        std::smatch m;
        auto s = text;
        while (std::regex_search(s, m, func_sig)) {
            std::string params = m[2].str();

            if (trim_copy(params).empty()) { s = m.suffix().str(); continue; }

            int commas = 0;
            int angle = 0;
            for (char c : params) {
                if (c == '<') ++angle;
                else if (c == '>') if (angle > 0) --angle;
                else if (c == ',' && angle == 0) ++commas;
            }
            int paramCount = commas + 1;
            if (paramCount > 4) {
                std::ostringstream ex;
                ex << f.string() << " -> params=" << paramCount << " signature_preview=\"" << trim_copy(m[0].str()) << "\"";
                offenders.push_back(ex.str());
            }
            s = m.suffix().str();
            if (offenders.size() >= 20) break;
        }
        if (offenders.size() >= 20) break;
    }
    if (!offenders.empty()) {
        std::ostringstream msg;
        msg << "Functions with too many parameters (>4) detected (heuristic). Examples:\n";
        for (auto& e : offenders) msg << "  " << e << "\n";
        msg << "Consider grouping parameters or introducing parameter objects.";
        throw std::runtime_error(msg.str());
    }
}


REGISTER_TEST(TU_test_40_casts_and_multi_inherit) {
    auto files = collect_source_files();
    std::vector<std::string> dynamic_cast_sites;
    std::vector<std::string> multi_inherit_sites;
    std::regex dyn_re(R"(dynamic_cast\s*<)");
    std::regex class_multi_re(R"(\bclass\s+\w+\s*:\s*[^;{]+,)");
    for (auto& f : files) {
        auto lines = read_lines(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            auto L = lines[i];
            if (std::regex_search(L, dyn_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy(L);
                dynamic_cast_sites.push_back(ex.str());
            }
            if (std::regex_search(L, class_multi_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy(L);
                multi_inherit_sites.push_back(ex.str());
            }
        }
    }
    if (!dynamic_cast_sites.empty() || !multi_inherit_sites.empty()) {
        std::ostringstream msg;
        if (!dynamic_cast_sites.empty()) {
            msg << "dynamic_cast usage detected (consider design alternatives / polymorphic interface):\n";
            for (auto& e : dynamic_cast_sites) msg << "  " << e << "\n";
        }
        if (!multi_inherit_sites.empty()) {
            msg << "Multiple inheritance (implementation) declarations detected (prefer single inheritance for implementation):\n";
            for (auto& e : multi_inherit_sites) msg << "  " << e << "\n";
        }
        throw std::runtime_error(msg.str());
    }
}