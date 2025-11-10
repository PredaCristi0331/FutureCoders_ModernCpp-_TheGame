export module thegame;

#include <vector>
#include <array>
#include <random>
#include <ranges>
#include <algorithm>
#include <memory>
#include <functional>
#include <span>
#include <concepts>
#include <utility>

// A concept for the Card-like type used by the engine
export template<typename T>
concept CardLike = requires(T a) {
    { a.value } -> std::convertible_to<int>;
};

// Templated engine as a template
export namespace thegame {

    // Simple Card template example
    export struct DefaultCard {
        int value;
    };

    // Generic Game engine as a template
    export template<CardLike CardT>
        requires std::is_move_constructible_v<CardT>
    class GameT {
    public:
        explicit GameT(int seed = 0);

        // play a card by value onto a pile index
        bool play_card(const CardT& card, int pile_index);

        // Query piles
        std::array<int, 4> get_piles() const noexcept;

        // Utility: create cards using variadic template convenience
        template<typename... Vs>
        static std::vector<CardT> make_cards(Vs&&... vs);

    private:
        std::mt19937 rng_;
        std::vector<CardT> history_;
        std::array<int, 4> piles_;
        bool is_playable_impl(int value, int pile_idx) const noexcept;
    };

    // export an alias for the default instantiation
    export using Game = GameT<DefaultCard>;
}