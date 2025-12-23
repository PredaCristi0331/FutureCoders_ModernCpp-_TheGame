#include "Pile.h"

Pile::Pile(PileType type)
    : type_(type)
    , initialValue_(type == PileType::Ascending ? 1 : 100) {

    // Lambda pentru validare bazatã pe tip
    if (type_ == PileType::Ascending) {
        validator_ = [](const Card& newCard, const Card& topCard) {
            // Poate plasa dacã e mai mare sau exact 10 mai mic (backward trick)
            return newCard.getValue() > topCard.getValue() ||
                newCard.getValue() == topCard.getValue() - 10;
            };
    }
    else {
        validator_ = [](const Card& newCard, const Card& topCard) {
            // Poate plasa dacã e mai mic sau exact 10 mai mare (backward trick)
            return newCard.getValue() < topCard.getValue() ||
                newCard.getValue() == topCard.getValue() + 10;
            };
    }
}

bool Pile::canPlaceCard(const Card& card) const {
    if (cards_.empty()) {
        return true; // Orice carte poate fi plasatã pe un pile gol
    }

    return validator_(card, *cards_.back());
}

bool Pile::placeCard(std::shared_ptr<Card> card) {
    if (!canPlaceCard(*card)) {
        return false;
    }

    cards_.push_back(std::move(card));
    return true;
}

std::optional<int> Pile::getTopValue() const {
    if (cards_.empty()) {
        return initialValue_;
    }
    return cards_.back()->getValue();
}

void Pile::reset() {
    cards_.clear();
}