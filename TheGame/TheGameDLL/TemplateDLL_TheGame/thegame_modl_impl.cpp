#include <utility>
#include <vector>
#include <array>
#include <random>

namespace thegame {

    // Use typename+requires instead of abbreviated template syntax.
    template<typename CardT>
        requires CardLike<CardT>&& std::is_move_constructible_v<CardT>
    GameT<CardT>::GameT(int seed) : rng_(seed), piles_{ 1,1,100,100 } {
        // nothing else to initialize here
    }

    template<typename CardT>
        requires CardLike<CardT>&& std::is_move_constructible_v<CardT>
    bool GameT<CardT>::is_playable_impl(int value, int pile_idx) const noexcept {
        int top = piles_[pile_idx];
        if (pile_idx == 0 || pile_idx == 1) {
            return (value > top) || (value == top - 10);
        }
        else {
            return (value < top) || (value == top + 10);
        }
    }

    template<typename CardT>
        requires CardLike<CardT>&& std::is_move_constructible_v<CardT>
    bool GameT<CardT>::play_card(const CardT& card, int pile_index) {
        if (pile_index < 0 || pile_index > 3) return false;
        if (!is_playable_impl(card.value, pile_index)) return false;
        // Demonstrate move semantics: copy to local then move into history
        CardT local = card;
        history_.push_back(std::move(local));
        piles_[pile_index] = card.value;
        return true;
    }

    template<typename CardT>
        requires CardLike<CardT>&& std::is_move_constructible_v<CardT>
    std::array<int, 4> GameT<CardT>::get_piles() const noexcept {
        return piles_;
    }

    template<typename CardT>
        requires CardLike<CardT>&& std::is_move_constructible_v<CardT>
    template<typename... Vs>
    std::vector<CardT> GameT<CardT>::make_cards(Vs&&... vs) {
        std::vector<CardT> v;
        // variadic template expansion to emplace_back with perfect forwarding
        (v.emplace_back(CardT{ static_cast<int>(std::forward<Vs>(vs)) }), ...);
        return v;
    }

    // Explicit instantiation for module-local DefaultCard
    template class GameT<DefaultCard>;
}
