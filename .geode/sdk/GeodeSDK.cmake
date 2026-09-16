# Stub file — build-geode-mod action provides the real path at build time
if(NOT DEFINED GEODE_SDK_DIR)
    set(GEODE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." CACHE PATH "Geode SDK root")
endif()

include("${GEODE_SDK_DIR}/cmake/GeodeSDK.cmake")
