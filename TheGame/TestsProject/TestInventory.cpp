#include "TestInventory.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

void testInventoryAdd() {
    std::vector<int> hand;
    hand.push_back(45);
    hand.push_back(22);
    assert(hand.size() == 2);
    std::cout << "Inventory add passed\n";
}

void testInventoryRemove() {
    std::vector<int> hand{ 10, 20, 30 };
    hand.erase(std::remove(hand.begin(), hand.end(), 20), hand.end());
    assert(hand.size() == 2);
    std::cout << "Inventory remove passed\n";
}

void testInventorySearch() {
    std::vector<int> hand{ 5, 17, 23, 42 };
    auto found = std::find(hand.begin(), hand.end(), 23);
    assert(found != hand.end());
    std::cout << "Inventory search passed\n";
}

void testInventorySort() {
    std::vector<int> hand{ 45, 12, 88 };
    std::sort(hand.begin(), hand.end());
    assert(hand[0] == 12 && hand[2] == 88);
    std::cout << "Inventory sort passed\n";
}