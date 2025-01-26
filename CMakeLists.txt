cmake_minimum_required(VERSION 3.28)

project(user VERSION 0.1.0)

option(USER_DEVELOPER "Enable ${PROJECT_NAME} developer mode" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(cmake/CompileOptions.cmake)

add_subdirectory(source)

if(USER_DEVELOPER)
    find_package(GTest REQUIRED)
    add_subdirectory(test)
endif()