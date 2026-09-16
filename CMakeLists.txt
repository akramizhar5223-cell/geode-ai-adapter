cmake_minimum_required(VERSION 3.21)
project(gd_fly_brain_ai)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include($ENV{GEODE_SDK}/cmake/GeodeSDK.cmake)

add_library(${PROJECT_NAME} SHARED src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GeodeSDK)
geode_configure_mod(${PROJECT_NAME})
