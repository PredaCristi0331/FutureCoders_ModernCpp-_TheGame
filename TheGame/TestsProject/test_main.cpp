#include "TestRunner.h"

// include all headers so tests auto-register
#include "TestUtils.h"
#include "TestGameLogic.h"
#include "TestInventory.h"
#include "TestPlayer.h"

int main() {
    // Run everything that registered via REGISTER_TEST
    TestRegistry::instance().runAll();
    return 0;
}