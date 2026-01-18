#pragma once

#pragma once
#include "Card.h"
#include "Export.h"
#include <vector>
#include <memory>
#include <optional>
#include <functional>

enum class PileType {
    Ascending,
    Descending
};

class THEGAME_API Pile {
private:
    std::vector<std::shared_ptr<Card>> cards_;
    PileType type_;
    int initialValue_;

    
    std::function<bool(const Card&, const Card&)> validator_;

public:
    explicit Pile(PileType type);

    
    Pile(Pile&& other) noexcept = default;
    Pile& operator=(Pile&& other) noexcept = default;

    Pile(const Pile&) = default;
    Pile& operator=(const Pile&) = default;

    [[nodiscard]] bool canPlaceCard(const Card& card) const;

    bool placeCard(std::shared_ptr<Card> card);

    [[nodiscard]] std::optional<int> getTopValue() const;

    [[nodiscard]] PileType getType() const noexcept { return type_; }

    [[nodiscard]] size_t size() const noexcept { return cards_.size(); }

    void reset();

    
    [[nodiscard]] auto getCards() const -> const std::vector<std::shared_ptr<Card>>& {
        return cards_;
    }
};