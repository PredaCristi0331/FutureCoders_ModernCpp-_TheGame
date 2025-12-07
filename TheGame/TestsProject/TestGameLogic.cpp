#include "TestGameLogic.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <optional>

// pentru test: funcție simplă care verifică mutarea pe teanc ascendent
bool canPlayAscending(int top, int candidate) {
    return candidate > top || candidate == top - 10;
}

bool canPlayDescending(int top, int candidate) {
    return candidate < top || candidate == top + 10;
}

void testAscendingRuleLogic() {
    assert(canPlayAscending(23, 25));   // valoare mai mare
    assert(!canPlayAscending(23, 18));  // nu se poate normal
    std::cout << "Asc rule logic passed\n";
}

void testDescendingRuleLogic() {
    assert(canPlayDescending(70, 67));  // valoare mai mică
    assert(!canPlayDescending(70, 72)); // nu merge
    std::cout << "Desc rule logic passed\n";
}

void testBackwardsTrickLogic() {
    assert(canPlayAscending(63, 53));   // backwards trick
    assert(canPlayDescending(44, 54));  // backwards trick
    std::cout << "Backwards trick logic passed\n";
}

void testWinLoseLogic() {
    auto winCondition = [](int cardsLeft) -> bool { return cardsLeft == 0; };
    auto loseCondition = [](std::vector<std::pair<int, std::vector<int>>> pilesWithHand) -> bool {
        // fiecare jucător trebuie să poată juca măcar 1 carte pe oricare teanc (logic minimală)
        for (const auto& [top, hand] : pilesWithHand) {
            bool canPlay = false;
            for (int card : hand) {
                if (canPlayAscending(top, card) || canPlayDescending(top, card))
                    canPlay = true;
            }
            if (!canPlay) return true; // pierde dacă nu poate plasa
        }
        return false;
        };

    assert(winCondition(0));
    assert(!winCondition(3));
    std::vector<std::pair<int, std::vector<int>>> pilesWithHand = { {55, {65,52}}, {80, {82,70}} };
    assert(!loseCondition(pilesWithHand)); // se poate juca
    std::vector<std::pair<int, std::vector<int>>> pilesWithHand2 = { {30, {15,12}}, {99, {11,10}} };
    assert(loseCondition(pilesWithHand2)); // nu se poate juca
    std::cout << "Win/lose logic passed\n";
}