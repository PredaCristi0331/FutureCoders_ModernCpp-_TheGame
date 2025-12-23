#include "Player.h"
#include <algorithm>
#include <ranges>

Player::Player(std::string name) : name_(std::move(name)) {}

Player::Player(Player&& other) noexcept
    : name_(std::move(other.name_))
    , hand_(std::move(other.hand_)) {
}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        name_ = std::move(other.name_);
        hand_ = std::move(other.hand_);
    }
    return *this;
}

void Player::addCard(std::unique_ptr<Card> card) {
    if (hand_.size() < MAX_HAND_SIZE) {
        hand_.push_back(std::move(card));

        // Sort hand using ranges
        std::ranges::sort(hand_, {}, [](const auto& card) {
            return card->getValue();
            });
    }
}

std::optional<std::unique_ptr<Card>> Player::playCard(size_t index) {
    if (index >= hand_.size()) {
        return std::nullopt;
    }

    auto card = std::move(hand_[index]);
    hand_.erase(hand_.begin() + index);
    return card;
}

std::vector<int> Player::getHandValues() const {
    std::vector<int> values;

    // Using ranges
    std::ranges::transform(hand_, std::back_inserter(values),
        [](const auto& card) { return card->getValue(); });

    return values;
}