cmake_minimum_required(VERSION 3.20)

# Executable
add_executable(TemplateDLL_TheGame
    main.cpp
)

target_link_libraries(TemplateDLL_TheGame
    PRIVATE
        TheGameDLL
)

target_compile_features(TemplateDLL_TheGame PUBLIC cxx_std_20)

# Compiler warnings
if(MSVC)
    target_compile_options(TemplateDLL_TheGame PRIVATE /W4 /permissive-)
else()
    target_compile_options(TemplateDLL_TheGame PRIVATE -Wall -Wextra -Wpedantic)
endif()