project(TemplateDLL_TheGame)

# Module support depends on compiler; assume recent MSVC/gcc/clang with C++20 modules support.
add_library(TemplateDLL_TheGame SHARED
    thegame_module_impl.cpp
    template_wrapper.cpp
    thegame.module.cppm
    template_api.h
)

target_include_directories(TemplateDLL_TheGame PUBLIC include)
target_compile_features(TemplateDLL_TheGame PUBLIC cxx_std_20)

if(UNIX)
    target_compile_definitions(TemplateDLL_TheGame PRIVATE THEGAMEDLL_UNIX)
endif()