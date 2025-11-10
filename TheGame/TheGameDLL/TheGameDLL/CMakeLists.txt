project(TheGameDLL)

add_library(TheGameDLL SHARED
    game.cpp
    thegame_api.h
)

target_include_directories(TheGameDLL PUBLIC include)
target_compile_features(TheGameDLL PUBLIC cxx_std_20)

# On Unix export symbol visibility for shared libraries
if(UNIX)
    target_compile_definitions(TheGameDLL PRIVATE THEGAMEDLL_UNIX)
endif()