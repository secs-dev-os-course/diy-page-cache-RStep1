get_filename_component(PAGE_CACHE_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(PAGE_CACHE_SOURCE_PATH "./${PROJECT_NAME}" ABSOLUTE)

file(GLOB_RECURSE PAGE_CACHE_SOURCES CONFIGURE_DEPENDS *.hpp *.cpp)

set(PAGE_CACHE_LIB_SOURCES ${PAGE_CACHE_SOURCES})

set(PAGE_CACHE_APP_SOURCES page_cache/app/Main.cpp)

add_library(${PROJECT_NAME} STATIC ${PAGE_CACHE_LIB_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PAGE_CACHE_INCLUDE_PATH})

cmake_policy(SET CMP0076 NEW) # avoid warning of relative paths translation

add_executable(${PROJECT_NAME}-app ${PAGE_CACHE_APP_SOURCES})
set_property(TARGET ${PROJECT_NAME}-app PROPERTY OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(
    ${PROJECT_NAME}-app PRIVATE
    ${PROJECT_NAME}
)