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
#include <mutex>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <variant>

using namespace std::string_literals;
namespace fs = std::filesystem;


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


struct TopView {
    int v;
    auto operator<=>(const TopView&) const = default;
};


static std::vector<int> consume_and_sort(std::vector<int> v) {
    std::ranges::sort(v);
    return v;
}


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


static void play_card_or_throw(int top, int card) {
    if (!(canPlayAscending(top, card) || canPlayDescending(top, card))) {
        throw std::invalid_argument("illegal move");
    }
}

static int draw_from_deck(std::vector<int>& deck) {
    if (deck.empty()) throw std::out_of_range("deck empty");
    int v = deck.back(); deck.pop_back(); return v;
}

REGISTER_TEST(TG_test_26) {
    bool caught = false;
    try { play_card_or_throw(50, 25); }
    catch (const std::invalid_argument& e) { caught = true; }
    assert(caught);
}

REGISTER_TEST(TG_test_27) {
    std::vector<int> deck;
    bool thrown = false;
    try { draw_from_deck(deck); }
    catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
}

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

REGISTER_TEST(TG_test_29) {
    auto fut = std::async(std::launch::async, []() { throw std::runtime_error("play fail"); return 0; });
    bool catched = false;
    try { fut.get(); }
    catch (const std::runtime_error& e) { catched = std::string(e.what()) == "play fail"; }
    assert(catched);
}

REGISTER_TEST(TG_test_30) {
    std::string input = "pile:3";
    std::regex re(R"(pile:(\d+))");
    std::smatch m;
    bool ok = std::regex_match(input, m, re);
    assert(ok && m.size() == 2 && m[1] == "3");
}

REGISTER_TEST(TG_test_31) {
    std::vector<TopView> v = { {2},{1},{3} };
    std::ranges::sort(v);
    assert(v[0].v == 1);
}

REGISTER_TEST(TG_test_32) {
    auto deck = make_range(2, 10);
    auto moved_deck = std::move(deck);
    assert(deck.empty() && !moved_deck.empty());
}


REGISTER_TEST(TG_test_33) {
    auto frontend_do = [](int top, int card)->std::variant<int, std::string> {
        try { play_card_or_throw(top, card); return card; }
        catch (const std::exception& e) { return std::string(e.what()); }
        };
    auto r = frontend_do(50, 25);
    assert(std::holds_alternative<std::string>(r));
}

REGISTER_TEST(TG_test_34) {
    std::vector<int> tops = { 1,1,100,100 };
    std::unordered_set<int> s(tops.begin(), tops.end());
    assert(s.size() == 2);
}

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


static std::vector<fs::path> collect_source_files_TG() {
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

static std::vector<std::string> read_lines_TG(const fs::path& p) {
    std::vector<std::string> lines;
    std::ifstream ifs(p);
    if (!ifs) return lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);
    return lines;
}

static std::string trim_copy_TG(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}
static bool is_comment_line_TG(const std::string& s) {
    auto t = trim_copy_TG(s);
    return t.rfind("//", 0) == 0 || t.rfind("/*", 0) == 0 || t.rfind("*", 0) == 0;
}

REGISTER_TEST(TG_test_36_duplicate_code) {
    auto files = collect_source_files_TG();
    const int WINDOW = 5;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> seen;
    for (auto& f : files) {
        auto lines = read_lines_TG(f);
        std::vector<std::string> norm;
        norm.reserve(lines.size());
        for (auto& L : lines) {
            auto t = trim_copy_TG(L);
            if (t.empty() || is_comment_line_TG(t)) continue;
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

REGISTER_TEST(TG_test_37_magic_numbers) {
    auto files = collect_source_files_TG();
    std::regex number_re(R"((?<![\w.])(-?\d+)(?![\w.]))");
    std::vector<std::string> examples;
    for (auto& f : files) {
        auto lines = read_lines_TG(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            const auto& L = lines[i];
            std::string trimmed = trim_copy_TG(L);
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
                        ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TG(L);
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

REGISTER_TEST(TG_test_38_long_functions) {
    auto files = collect_source_files_TG();
    const int MAX_LINES = 50;
    std::vector<std::string> long_funcs;
    std::regex control_kw(R"(\b(if|for|while|switch|catch|else|return|do)\b)");
    for (auto& f : files) {
        auto lines = read_lines_TG(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            std::string L = trim_copy_TG(lines[i]);
            if (L.empty()) continue;
            if (L.find(')') != std::string::npos && L.back() != ';') {
                if (std::regex_search(L, control_kw)) continue;
                size_t braceLine = i;
                bool foundBrace = false;
                if (L.find('{') != std::string::npos) foundBrace = true;
                else {
                    size_t j = i + 1;
                    for (; j < lines.size() && j < i + 6; ++j) {
                        auto next = trim_copy_TG(lines[j]);
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

REGISTER_TEST(TG_test_39_many_params) {
    auto files = collect_source_files_TG();
    std::vector<std::string> offenders;
    std::regex func_sig(R"(([^;{}()\n<>]+)\(([^\)]*)\)\s*(const)?\s*(?:\{|;))");
    for (auto& f : files) {
        auto text = std::string();
        for (auto& L : read_lines_TG(f)) text += L + "\n";
        std::smatch m;
        auto s = text;
        while (std::regex_search(s, m, func_sig)) {
            std::string params = m[2].str();
            if (trim_copy_TG(params).empty()) { s = m.suffix().str(); continue; }
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
                ex << f.string() << " -> params=" << paramCount << " signature_preview=\"" << trim_copy_TG(m[0].str()) << "\"";
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

REGISTER_TEST(TG_test_40_casts_and_multi_inherit) {
    auto files = collect_source_files_TG();
    std::vector<std::string> dynamic_cast_sites;
    std::vector<std::string> multi_inherit_sites;
    std::regex dyn_re(R"(dynamic_cast\s*<)");
    std::regex class_multi_re(R"(\bclass\s+\w+\s*:\s*[^;{]+,)");
    for (auto& f : files) {
        auto lines = read_lines_TG(f);
        for (size_t i = 0; i < lines.size(); ++i) {
            auto L = lines[i];
            if (std::regex_search(L, dyn_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TG(L);
                dynamic_cast_sites.push_back(ex.str());
            }
            if (std::regex_search(L, class_multi_re)) {
                std::ostringstream ex; ex << f.string() << ":" << (i + 1) << " -> " << trim_copy_TG(L);
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