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
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <variant>
#include<set>


using namespace std::string_literals;
namespace fs = std::filesystem;

template<std::integral T>
double avg_integrals(const std::vector<T>& v) {
    T s = 0;
    for (auto x : v) s += x;
    return double(s) / v.size();
}

// existing tests 01..35 ------------------------------------------------------

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

REGISTER_TEST(TP_test_35) {
    auto frontend = [](int pid, int max)->std::variant<int, std::string> {
        try { ensure_valid_player(pid, max); return pid; }
        catch (const std::exception& e) { return std::string(e.what()); }
        };
    auto r = frontend(9, 4);
    assert(std::holds_alternative<std::string>(r));
}

// -------------- Static-analysis style heuristic checks (TP_test_36..40) -----

static std::vector<fs::path> collect_source_files_TP() {
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

static std::vector<std::string> read_lines_TP(const fs::path& p) {
    std::vector<std::string> lines;
    std::ifstream ifs(p);
    if (!ifs) return lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);
    return lines;
}

static std::string trim_copy_TP(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}
static bool is_comment_line_TP(const std::string& s) {
    auto t = trim_copy_TP(s);
    return t.rfind("//", 0) == 0 || t.rfind("/*", 0) == 0 || t.rfind("*", 0) == 0;
}

REGISTER_TEST(TP_test_36_duplicate_code) {
    auto files = collect_source_files_TP();
    const int WINDOW = 5;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> seen;
    for (auto& f : files) {
        auto lines = read_lines_TP(f);
        std::vector<std::string> norm;
        norm.reserve(lines.size());
        for (auto& L : lines) {
            auto t = trim_copy_TP(L);
            if (t.empty() || is_comment_line_TP(t)) continue;
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

REGISTER_TEST(TP_test_37_magic_numbers) {
    auto files = collect_source_files_TP();
    std::regex number_re(R"((?<![\w.])(-?\d+)(?![\w.]))");
    std::vector<std::string> examples;
    for (auto& f : files) {
        auto lines = read_lines_TP(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            const auto& L = lines[i];
            std::string trimmed = trim_copy_TP(L);
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
                        ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TP(L);
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

REGISTER_TEST(TP_test_38_long_functions) {
    auto files = collect_source_files_TP();
    const int MAX_LINES = 50;
    std::vector<std::string> long_funcs;
    std::regex control_kw(R"(\b(if|for|while|switch|catch|else|return|do)\b)");
    for (auto& f : files) {
        auto lines = read_lines_TP(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string L = trim_copy_TP(lines[i]);
            if (L.empty()) continue;
            if (L.find(')') != std::string::npos && L.back() != ';') {
                if (std::regex_search(L, control_kw)) continue;
                size_t braceLine = i;
                bool foundBrace = false;
                if (L.find('{') != std::string::npos) foundBrace = true;
                else {
                    size_t j = i + 1;
                    for (; j < lines.size() && j < i + 6; ++j) {
                        auto next = trim_copy_TP(lines[j]);
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

REGISTER_TEST(TP_test_39_many_params) {
    auto files = collect_source_files_TP();
    std::vector<std::string> offenders;
    std::regex func_sig(R"(([^;{}()\n<>]+)\(([^\)]*)\)\s*(const)?\s*(?:\{|;))");
    for (auto& f : files) {
        auto text = std::string();
        for (auto& L : read_lines_TP(f)) text += L + "\n";
        std::smatch m;
        auto s = text;
        while (std::regex_search(s, m, func_sig)) {
            std::string params = m[2].str();
            if (trim_copy_TP(params).empty()) { s = m.suffix().str(); continue; }
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
                ex << f.string() << " -> params=" << paramCount << " signature_preview=\"" << trim_copy_TP(m[0].str()) << "\"";
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

REGISTER_TEST(TP_test_40_casts_and_multi_inherit) {
    auto files = collect_source_files_TP();
    std::vector<std::string> dynamic_cast_sites;
    std::vector<std::string> multi_inherit_sites;
    std::regex dyn_re(R"(dynamic_cast\s*<)");
    std::regex class_multi_re(R"(\bclass\s+\w+\s*:\s*[^;{]+,)");
    for (auto& f : files) {
        auto lines = read_lines_TP(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            auto L = lines[i];
            if (std::regex_search(L, dyn_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TP(L);
                dynamic_cast_sites.push_back(ex.str());
            }
            if (std::regex_search(L, class_multi_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TP(L);
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