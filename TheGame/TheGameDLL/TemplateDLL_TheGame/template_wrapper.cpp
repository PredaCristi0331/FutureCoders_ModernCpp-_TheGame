#include "template_api.h"
#include "thegame_module.cppm"

#include <string>
#include <regex>
#include <memory>

// Use the exported module alias
using namespace thegame;

extern "C" {

    TGT_GameHandle tg_template_create(int seed) {
        // Use shared_ptr for demonstration of smart pointers across DLL boundary
        auto g = std::make_shared<Game>(seed);
        return new std::shared_ptr<Game>(std::move(g));
    }

    void tg_template_destroy(TGT_GameHandle h) {
        if (!h) return;
        auto p = static_cast<std::shared_ptr<Game>*>(h);
        delete p;
    }

    int tg_template_play(TGT_GameHandle h, int card_value, int pile_index) {
        if (!h) return 0;
        auto p = static_cast<std::shared_ptr<Game>*>(h);
        thegame::DefaultCard c{ card_value };
        return (*p)->play_card(c, pile_index) ? 1 : 0;
    }

    void tg_template_get_piles(TGT_GameHandle h, int out_piles[4]) {
        if (!h) return;
        auto p = static_cast<std::shared_ptr<Game>*>(h);
        auto arr = (*p)->get_piles();
        for (size_t i = 0; i < 4; ++i) out_piles[i] = arr[i];
    }

    int tg_template_execute(TGT_GameHandle h, const char* command) {
        if (!h || !command) return 0;
        auto p = static_cast<std::shared_ptr<Game>*>(h);
        // regex parsing
        static const std::regex rx(R"(^\s*play\s+(\d{1,2})\s+(\d)\s*$)", std::regex::icase);
        std::smatch m;
        std::string s(command);
        if (std::regex_search(s, m, rx)) {
            int value = std::stoi(m[1].str());
            int pile = std::stoi(m[2].str());
            thegame::DefaultCard c{ value };
            return (*p)->play_card(c, pile) ? 1 : 0;
        }
        return 0;
    }

} // extern "C"