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
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std::string_literals;
namespace fs = std::filesystem;

// --- helpers & existing tests ------------------------------------------------

static std::deque<int> make_deque_moved(std::vector<int> v) {
    std::deque<int> d;
    for (auto& x : v) d.push_back(std::move(x));
    return d;
}

template<typename Container>
auto container_sum(const Container& c) {
    using T = typename Container::value_type;
    T s{};
    for (const auto& x : c) s += x;
    return s;
}

template<typename... Ts>
auto make_vec(Ts... xs) {
    return std::vector{ xs... };
}

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

REGISTER_TEST(TI_test_27) {
    std::string code = "ITM-001";
    std::regex re(R"(ITM-\d{3})");
    assert(std::regex_match(code, re));
}

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

REGISTER_TEST(TI_test_30) {
    std::vector<int> v;
    bool caught = false;
    try {
        throw std::bad_alloc();
    }
    catch (const std::bad_alloc&) { caught = true; }
    assert(caught);
}

REGISTER_TEST(TI_test_31) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("compact fail"); return 0; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error&) { catched = true; }
    assert(catched);
}

REGISTER_TEST(TI_test_32) {
    std::vector<int> v = { 1,2,3 };
    auto it = v.begin();
    v.push_back(4); // may invalidate
    bool ok = (it != v.end());
    (void)ok;
    assert(true);
}

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

REGISTER_TEST(TI_test_34) {
    std::vector<int> hand = { 2,2,3,3 };
    auto backup = hand;
    try {
        hand.erase(std::remove(hand.begin(), hand.end(), 42), hand.end());
    }
    catch (...) { hand = backup; }
    assert(!hand.empty());
}

REGISTER_TEST(TI_test_35) {
    auto v = make_vec(11, 22, 33);
    std::ostringstream oss;
    for (size_t i = 0; i < v.size(); ++i) { if (i) oss << ','; oss << v[i]; }
    std::string s = oss.str();
    std::regex re(R"(\d+,\d+,\d+)");
    assert(std::regex_match(s, re));
}


static std::vector<fs::path> collect_source_files_TI() {
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

static std::vector<std::string> read_lines_TI(const fs::path& p) {
    std::vector<std::string> lines;
    std::ifstream ifs(p);
    if (!ifs) return lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);
    return lines;
}

static std::string trim_copy_TI(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}
static bool is_comment_line_TI(const std::string& s) {
    auto t = trim_copy_TI(s);
    return t.rfind("//", 0) == 0 || t.rfind("/*", 0) == 0 || t.rfind("*", 0) == 0;
}

REGISTER_TEST(TI_test_36_duplicate_code) {
    auto files = collect_source_files_TI();
    const int WINDOW = 5;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> seen;
    for (auto& f : files) {
        auto lines = read_lines_TI(f);
        std::vector<std::string> norm;
        norm.reserve(lines.size());
        for (auto& L : lines) {
            auto t = trim_copy_TI(L);
            if (t.empty() || is_comment_line_TI(t)) continue;
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

REGISTER_TEST(TI_test_37_magic_numbers) {
    auto files = collect_source_files_TI();
    std::regex number_re(R"((?<![\w.])(-?\d+)(?![\w.]))");
    std::vector<std::string> examples;
    for (auto& f : files) {
        auto lines = read_lines_TI(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            const auto& L = lines[i];
            std::string trimmed = trim_copy_TI(L);
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
                        ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TI(L);
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

REGISTER_TEST(TI_test_38_long_functions) {
    auto files = collect_source_files_TI();
    const int MAX_LINES = 50;
    std::vector<std::string> long_funcs;
    std::regex control_kw(R"(\b(if|for|while|switch|catch|else|return|do)\b)");
    for (auto& f : files) {
        auto lines = read_lines_TI(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string L = trim_copy_TI(lines[i]);
            if (L.empty()) continue;
            if (L.find(')') != std::string::npos && L.back() != ';') {
                if (std::regex_search(L, control_kw)) continue;
                size_t braceLine = i;
                bool foundBrace = false;
                if (L.find('{') != std::string::npos) foundBrace = true;
                else {
                    size_t j = i + 1;
                    for (; j < lines.size() && j < i + 6; ++j) {
                        auto next = trim_copy_TI(lines[j]);
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

REGISTER_TEST(TI_test_39_many_params) {
    auto files = collect_source_files_TI();
    std::vector<std::string> offenders;
    std::regex func_sig(R"(([^;{}()\n<>]+)\(([^\)]*)\)\s*(const)?\s*(?:\{|;))");
    for (auto& f : files) {
        auto text = std::string();
        for (auto& L : read_lines_TI(f)) text += L + "\n";
        std::smatch m;
        auto s = text;
        while (std::regex_search(s, m, func_sig)) {
            std::string params = m[2].str();
            if (trim_copy_TI(params).empty()) { s = m.suffix().str(); continue; }
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
                ex << f.string() << " -> params=" << paramCount << " signature_preview=\"" << trim_copy_TI(m[0].str()) << "\"";
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

REGISTER_TEST(TI_test_40_casts_and_multi_inherit) {
    auto files = collect_source_files_TI();
    std::vector<std::string> dynamic_cast_sites;
    std::vector<std::string> multi_inherit_sites;
    std::regex dyn_re(R"(dynamic_cast\s*<)");
    std::regex class_multi_re(R"(\bclass\s+\w+\s*:\s*[^;{]+,)");
    for (auto& f : files) {
        auto lines = read_lines_TI(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            auto L = lines[i];
            if (std::regex_search(L, dyn_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TI(L);
                dynamic_cast_sites.push_back(ex.str());
            }
            if (std::regex_search(L, class_multi_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TI(L);
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