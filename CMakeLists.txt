cmake_minimum_required(VERSION 3.21)
project(gd_ai_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This is the CORRECT format for build-geode-mod action
include("${CMAKE_CURRENT_SOURCE_DIR}/.geode/sdk/GeodeSDK.cmake")

add_library(${PROJECT_NAME} SHARED src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GeodeSDK)
geode_configure_mod(${PROJECT_NAME})
