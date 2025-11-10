#pragma once

#ifdef _WIN32
#ifdef THEGAMEDLL_EXPORTS
#define THEGAMEDLL_API __declspec(dllexport)
#else
#define THEGAMEDLL_API __declspec(dllimport)
#endif
#else
#ifdef THEGAMEDLL_UNIX
#define THEGAMEDLL_API __attribute__((visibility("default")))
#else
#define THEGAMEDLL_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Opaque handle to a game instance
    typedef void* TG_GameHandle;

    // Create/destroy
    THEGAMEDLL_API TG_GameHandle tg_create_game(int seed);
    THEGAMEDLL_API void tg_destroy_game(TG_GameHandle h);

    // Play a card by integer value onto pile index (0-3). Returns 1 if success, 0 otherwise.
    THEGAMEDLL_API int tg_play_card(TG_GameHandle h, int card_value, int pile_index);

    // Get the 4 pile top values into `out_piles` (must have space for 4 ints).
    THEGAMEDLL_API void tg_get_piles(TG_GameHandle h, int out_piles[4]);

    // Parse a text command like "play 12 on 2" or "play 5 0" and execute it.
    // Returns 1 on success, 0 on failure.
    THEGAMEDLL_API int tg_execute_command(TG_GameHandle h, const char* command);

#ifdef __cplusplus
}
#endif
