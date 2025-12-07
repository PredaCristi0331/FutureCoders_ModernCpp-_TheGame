#include "TestUtils.h"
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>
#include <optional>
#include <variant>
#include <iostream>
#include <cassert>

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim))
        result.push_back(token);
    return result;
}

void testSplit() {
    auto tokens = split("44;1;99", ';');
    assert(tokens.size() == 3 && tokens[2] == "99");
    std::cout << "Split passed\n";
}

void testShuffle() {
    std::vector<int> deck{ 2,3,4,5,6,7,8,9,10 };
    std::shuffle(deck.begin(), deck.end(), std::mt19937{ std::random_device{}() });
    assert(deck.size() == 9);
    std::cout << "Shuffle passed\n";
}

void testOptional() {
    std::optional<int> opt;
    assert(!opt.has_value());
    opt = 42;
    assert(opt.value() == 42);
    std::cout << "Optional passed\n";
}

void testVariant() {
    std::variant<int, std::string> v = 17;
    assert(std::holds_alternative<int>(v));
    v = std::string("carte");
    assert(std::get<std::string>(v) == "carte");
    std::cout << "Variant passed\n";
}