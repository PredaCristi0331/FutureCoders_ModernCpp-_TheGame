#include "Deck.h"
#include <algorithm>
#include <random>

Deck::Deck() : rng_(std::random_device{}()) {
    
    for (int i = 2; i <= 99; ++i) {
        cards_.push_back(std::make_unique<Card>(i));
    }
    shuffle();
}

void Deck::shuffle() {
    std::ranges::shuffle(cards_, rng_);
}

std::optional<std::unique_ptr<Card>> Deck::drawCard() {
    if (cards_.empty()) {
        return std::nullopt;
    }

    auto card = std::move(cards_.back());
    cards_.pop_back();
    return card;
}