#include "Pile.h"

Pile::Pile(PileType type)
    : type_(type)
    , initialValue_(type == PileType::Ascending ? 1 : 100) {

    
    if (type_ == PileType::Ascending) {
        validator_ = [](const Card& newCard, const Card& topCard) {
            
            return newCard.getValue() > topCard.getValue() ||
                newCard.getValue() == topCard.getValue() - 10;
            };
    }
    else {
        validator_ = [](const Card& newCard, const Card& topCard) {
            
            return newCard.getValue() < topCard.getValue() ||
                newCard.getValue() == topCard.getValue() + 10;
            };
    }
}

bool Pile::canPlaceCard(const Card& card) const {
    if (cards_.empty()) {
        return true; 
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