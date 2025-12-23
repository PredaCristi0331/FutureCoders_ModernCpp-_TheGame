#pragma once

#pragma once
#include "Card.h"
#include "Export.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <ranges>
#include <random>

class THEGAME_API Deck {
private:
    std::vector<std::unique_ptr<Card>> cards_;
    std::mt19937 rng_;

public:
    Deck();

    // Move semantics
    Deck(Deck&& other) noexcept = default;
    Deck& operator=(Deck&& other) noexcept = default;

    // Delete copy (unique ownership)
    Deck(const Deck&) = delete;
    Deck& operator=(const Deck&) = delete;

    void shuffle();

    [[nodiscard]] std::optional<std::unique_ptr<Card>> drawCard();

    [[nodiscard]] size_t size() const noexcept { return cards_.size(); }

    [[nodiscard]] bool isEmpty() const noexcept { return cards_.empty(); }

    // Ranges support
    [[nodiscard]] auto getCardValues() const {
        return cards_
            | std::views::transform([](const auto& card) { return card->getValue(); });
    }

    // Variadic template pentru creare deck personalizat
    template<typename...  Values>
    static Deck createCustomDeck(Values... values) {
        Deck deck;
        deck.cards_.clear();
        (deck.cards_.push_back(std::make_unique<Card>(values)), ...);
        return deck;
    }
};