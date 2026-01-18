#pragma once

#pragma once
#include "Export.h"
#include <compare>
#include <optional>
#include <string>


template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

class THEGAME_API Card {
private:
    int value_;

public:
    
    explicit Card(const int& value) : value_(value) {}

    
    Card(Card&& other) noexcept : value_(std::exchange(other.value_, 0)) {}

    Card& operator=(Card&& other) noexcept {
        if (this != &other) {
            value_ = std::exchange(other.value_, 0);
        }
        return *this;
    }

    
    Card(const Card& other) = default;
    Card& operator=(const Card& other) = default;

    
    auto operator<=>(const Card& other) const = default;

    [[nodiscard]] int getValue() const noexcept { return value_; }

    
    template<Numeric T>
    [[nodiscard]] bool isMultipleOf(const T& divisor) const {
        return value_ % static_cast<int>(divisor) == 0;
    }

    [[nodiscard]] std::string toString() const;
};