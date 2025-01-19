cmake_minimum_required(VERSION 3.28)

project(page_cache VERSION 0.1.0)

option(PAGE_CACHE_DEVELOPER "Enable ${PROJECT_NAME} developer mode" OFF)
option(PAGE_CACHE_BENCHMARK "Enable ${PROJECT_NAME} benchmark module" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(cmake/CompileOptions.cmake)

add_subdirectory(source)

if(PAGE_CACHE_DEVELOPER)
    find_package(GTest REQUIRED)
    add_subdirectory(test)
endif()