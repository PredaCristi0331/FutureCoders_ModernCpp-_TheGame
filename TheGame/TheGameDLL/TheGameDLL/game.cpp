#define THEGAMEDLL_EXPORTS  // Quick fix: prefer adding this in project settings instead

#include "thegame_api.h"

#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <cassert>
#include <functional>
#include <span>

// Simplified representation of a card (value 2..99)
struct Card {
    int value;
};

// Game rules (simplified):
// - 4 piles: 0 and 1 are ascending (start at 1), 2 and 3 are descending (start at 100).
// - Ascending pile: play card with value > top OR exactly top - 10 (the special reverse rule).
// - Descending pile: play card with value < top OR exactly top + 10 (special reverse rule).

class Game {
public:
    Game(int seed) : rng(seed), piles_{ 1,1,100,100 } {
        init_deck();
        // deal nothing; deck left for illustration
    }

    // Play by value to pile idx; uses move semantics to show moving a card into pile history
    bool play_card(int value, int pile_idx) {
        if (pile_idx < 0 || pile_idx > 3) return false;
        if (!is_playable(value, pile_idx)) return false;

        // move semantics: create a Card then move into history vector
        Card c{ value };
        pile_history_.push_back(std::move(c));
        piles_[pile_idx] = value;
        return true;
    }

    std::array<int, 4> get_piles() const {
        return piles_;
    }

    // Execute text command using regex + parsing
    bool execute_command(std::string_view cmd) {
        static const std::regex rx(R"(^\s*play\s+(\d{1,2})\s*(?:on)?\s*(\d)\s*$)", std::regex::icase);
        std::smatch m;
        std::string s(cmd);
        if (std::regex_search(s, m, rx)) {
            int value = std::stoi(m[1].str());
            int pile = std::stoi(m[2].str());
            return play_card(value, pile) ? true : false;
        }
        return false;
    }

private:
    bool is_playable(int value, int pile_idx) const {
        int top = piles_[pile_idx];
        if (pile_idx == 0 || pile_idx == 1) { // ascending
            return (value > top) || (value == top - 10);
        }
        else { // descending
            return (value < top) || (value == top + 10);
        }
    }

    void init_deck() {
        deck_.clear();
        for (int v = 2; v <= 99; ++v) deck_.push_back(v);
        std::ranges::shuffle(deck_, rng);
    }

    std::mt19937 rng;
    std::vector<int> deck_;
    std::vector<Card> pile_history_; // demonstrates move semantics into vector
    std::array<int, 4> piles_;
};

// C API wrappers
extern "C" {

    THEGAMEDLL_API TG_GameHandle tg_create_game(int seed) {
        return new Game(seed);
    }

    THEGAMEDLL_API void tg_destroy_game(TG_GameHandle h) {
        if (!h) return;
        delete static_cast<Game*>(h);
    }

    THEGAMEDLL_API int tg_play_card(TG_GameHandle h, int card_value, int pile_index) {
        if (!h) return 0;
        Game* g = static_cast<Game*>(h);
        return g->play_card(card_value, pile_index) ? 1 : 0;
    }

    THEGAMEDLL_API void tg_get_piles(TG_GameHandle h, int out_piles[4]) {
        if (!h) return;
        auto g = static_cast<Game*>(h);
        auto p = g->get_piles();
        for (size_t i = 0; i < 4; ++i) out_piles[i] = p[i];
    }

    THEGAMEDLL_API int tg_execute_command(TG_GameHandle h, const char* command) {
        if (!h || !command) return 0;
        auto g = static_cast<Game*>(h);
        return g->execute_command(command) ? 1 : 0;
    }

} // extern "C"