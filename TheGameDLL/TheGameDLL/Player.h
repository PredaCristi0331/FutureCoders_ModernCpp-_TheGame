#pragma once
#include "Card.h"
#include "Export.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <array>

class THEGAME_API Player {
private:
    std::string name_;
    std::vector<std::unique_ptr<Card>> hand_;
    static constexpr size_t MAX_HAND_SIZE = 7;

public:
    explicit Player(std::string name);

   
    Player(Player&& other) noexcept;
    Player& operator=(Player&& other) noexcept;

    
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    [[nodiscard]] const std::string& getName() const noexcept { return name_; }

    void addCard(std::unique_ptr<Card> card);

    [[nodiscard]] std::optional<std::unique_ptr<Card>> playCard(size_t index);

    [[nodiscard]] size_t getHandSize() const noexcept { return hand_.size(); }

    [[nodiscard]] bool hasCards() const noexcept { return !hand_.empty(); }

    [[nodiscard]] std::vector<int> getHandValues() const;

    
    template<typename Predicate>
    [[nodiscard]] auto findCards(Predicate pred) const {
        return hand_
            | std::views::filter([&pred](const auto& card) {
            return pred(*card);
                })
            | std::views::transform([](const auto& card) {
            return card->getValue();
                });
    }
};
