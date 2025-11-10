#pragma once

#ifdef _WIN32
#ifdef TEMPLATEGAMEDLL_EXPORTS
#define TEMPLATEGAMEDLL_API __declspec(dllexport)
#else
#define TEMPLATEGAMEDLL_API __declspec(dllimport)
#endif
#else
#ifdef THEGAMEDLL_UNIX
#define TEMPLATEGAMEDLL_API __attribute__((visibility("default")))
#else
#define TEMPLATEGAMEDLL_API
#endif
#endif

extern "C" {

    // Opaque handle
    typedef void* TGT_GameHandle;

    // create/destroy
    TEMPLATEGAMEDLL_API TGT_GameHandle tg_template_create(int seed);
    TEMPLATEGAMEDLL_API void tg_template_destroy(TGT_GameHandle h);

    // play/expose piles
    TEMPLATEGAMEDLL_API int tg_template_play(TGT_GameHandle h, int card_value, int pile_index);
    TEMPLATEGAMEDLL_API void tg_template_get_piles(TGT_GameHandle h, int out_piles[4]);

    // Convenience: parse command string using regex (e.g., "play 12 0")
    TEMPLATEGAMEDLL_API int tg_template_execute(TGT_GameHandle h, const char* command);

}