get_filename_component(PAGE_CACHE_TEST_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(PAGE_CACHE_TEST_SOURCE_PATH "./${PROJECT_NAME}" ABSOLUTE)

set(PAGE_CACHE_TEST_TARGET "${PROJECT_NAME}-test")

file(GLOB_RECURSE PAGE_CACHE_TEST_SOURCES CONFIGURE_DEPENDS *.hpp *.cpp)

add_executable(${PAGE_CACHE_TEST_TARGET} ${PAGE_CACHE_TEST_SOURCES})

target_include_directories(${PAGE_CACHE_TEST_TARGET} PUBLIC ${PAGE_CACHE_TEST_INCLUDE_PATH})

target_link_libraries(
    ${PAGE_CACHE_TEST_TARGET} PRIVATE
    ${PROJECT_NAME}
    GTest::gtest
    GTest::gmock
)

gtest_discover_tests(${PAGE_CACHE_TEST_TARGET})