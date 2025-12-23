#include "Card.h"
#include <format>

std::string Card::toString() const {
    return std::format("Card({})", value_);
}