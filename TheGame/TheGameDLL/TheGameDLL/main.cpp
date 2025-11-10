#include <iostream>
#include <array>

// TheGameDLL
#include "thegame_api.h"
// TemplateDLL (use relative path to sibling project)
#include "../TemplateDLL_TheGame/template_api.h"

int main() {
    // Use TheGameDLL
    TG_GameHandle g1 = tg_create_game(42);
    int ok = tg_play_card(g1, 12, 0);
    std::array<int, 4> piles1{};
    tg_get_piles(g1, piles1.data());
    std::cout << "TheGameDLL played 12 on pile0 -> " << ok << "\n";
    std::cout << "Piles: ";
    for (int v : piles1) std::cout << v << " ";
    std::cout << "\n";

    // Use TemplateDLL
    TGT_GameHandle g2 = tg_template_create(100);
    tg_template_play(g2, 95, 3);
    int piles2[4];
    tg_template_get_piles(g2, piles2);
    std::cout << "TemplateDLL piles: ";
    for (int i = 0; i < 4; ++i) std::cout << piles2[i] << " ";
    std::cout << "\n";

    // Execute commands with regex parsing
    tg_execute_command(g1, "play 15 0"); // returns 1/0
    tg_template_execute(g2, "play 85 3");

    tg_destroy_game(g1);
    tg_template_destroy(g2);

    return 0;
}