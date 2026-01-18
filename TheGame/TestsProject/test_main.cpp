#include "TestRunner.h"


#include "TestUtils.h"
#include "TestGameLogic.h"
#include "TestInventory.h"
#include "PlayerTest.h"

int main() {
    
    TestRegistry::instance().runAll();
    return 0;
}