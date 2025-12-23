cmake_minimum_required(VERSION 3.20)

# DLL Project
add_library(TheGameDLL SHARED
    Card.cpp
    Deck.cpp
    Pile.cpp
    Player.cpp
    Game. cpp
    Logger.cpp
    Validators.cpp
)

target_include_directories(TheGameDLL
    PUBLIC
        $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(TheGameDLL
    PRIVATE
        THEGAME_EXPORTS
)

# C++20 features
target_compile_features(TheGameDLL PUBLIC cxx_std_20)

# Compiler warnings
if(MSVC)
    target_compile_options(TheGameDLL PRIVATE /W4 /permissive-)
else()
    target_compile_options(TheGameDLL PRIVATE -Wall -Wextra -Wpedantic)
endif()