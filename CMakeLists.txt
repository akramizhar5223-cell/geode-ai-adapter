cmake_minimum_required(VERSION 3.21)
project(FlyBrainAI VERSION 1.0.0)

# Enable C++17 for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/ai_system.cpp
    src/fly_brain_neural_net.cpp
    src/fly_brain_adapter.cpp
    src/pause_menu_hook.cpp
    src/player_analyzer.cpp
    src/learning_engine.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add optimization flags for performance
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /O2 /fp:fast)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -ffast-math)
endif()

# Geode SDK
include(FetchContent)
FetchContent_Declare(geode URL https://github.com/geode-sdk/geode/releases/download/v2.2.0/geode-2.2.0.tar.gz)
FetchContent_MakeAvailable(geode)

target_link_libraries(${PROJECT_NAME} PRIVATE geode-sdk)

create_geode_mod(${PROJECT_NAME})
