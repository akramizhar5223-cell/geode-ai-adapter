cmake_minimum_required(VERSION 3.21)
project(gd_ai_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ CORRECT: Let the action set the path automatically
if(DEFINED ENV{GEODE_SDK})
  include($ENV{GEODE_SDK}/cmake/GeodeSDK.cmake)
else()
  message(FATAL_ERROR "GEODE_SDK not set — use build-geode-mod action!")
endif()

add_library(${PROJECT_NAME} SHARED src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GeodeSDK)
geode_configure_mod(${PROJECT_NAME})
