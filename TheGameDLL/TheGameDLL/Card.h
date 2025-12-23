#pragma once

#pragma once
#include "Export.h"
#include <compare>
#include <optional>
#include <string>

// C++20 Concepts
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

class THEGAME_API Card {
private:
    int value_;

public:
    // Constructor cu const ref
    explicit Card(const int& value) : value_(value) {}

    // Move semantics
    Card(Card&& other) noexcept : value_(std::exchange(other.value_, 0)) {}

    Card& operator=(Card&& other) noexcept {
        if (this != &other) {
            value_ = std::exchange(other.value_, 0);
        }
        return *this;
    }

    // Copy semantics
    Card(const Card& other) = default;
    Card& operator=(const Card& other) = default;

    // C++20 three-way comparison
    auto operator<=>(const Card& other) const = default;

    [[nodiscard]] int getValue() const noexcept { return value_; }

    // Template method
    template<Numeric T>
    [[nodiscard]] bool isMultipleOf(const T& divisor) const {
        return value_ % static_cast<int>(divisor) == 0;
    }

    [[nodiscard]] std::string toString() const;
};