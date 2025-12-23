#pragma once

#pragma once

#ifdef _WIN32
#ifdef THEGAME_EXPORTS
#define THEGAME_API __declspec(dllexport)
#else
#define THEGAME_API __declspec(dllimport)
#endif
#else
#define THEGAME_API
#endif