cmake_minimum_required(VERSION 4.0)
project(karvikmatch)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/main.cpp
        src/asset/asset_registry.hpp
        src/asset/asset_definitions.hpp
        src/asset/asset_definitions.cpp
        src/scene/scene_registry.cpp
        src/scene/scene_registry.hpp
        src/scene/scene_id.hpp
        src/scene/scene.cpp
        src/scene/scene.hpp
        src/game/game_ctx.hpp
        src/scenedefs/splash_scene.cpp
        src/scenedefs/splash_scene.hpp
)

add_executable(karvikmatch ${SOURCE_FILES})

add_subdirectory(ext/spdlog-1.17.0)

target_include_directories(karvikmatch PRIVATE ext/raylib-6.0_win64_msvc16/include)
target_link_directories(karvikmatch PRIVATE ext/raylib-6.0_win64_msvc16/lib)

target_link_libraries(karvikmatch PRIVATE spdlog::spdlog raylib gdi32 opengl32 winmm)