add_library(page_cache SHARED 
    page_cache/FileAPI.cpp 
    page_cache/FIFOCache.cpp
)

target_include_directories(page_cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/page_cache)

set_target_properties(page_cache PROPERTIES
    VERSION 1.0
    SOVERSION 1
)

add_executable(app 
    user/app/Main.cpp 
    user/Linreg.cpp
)

target_link_libraries(app PRIVATE page_cache)

target_include_directories(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/page_cache
    ${CMAKE_CURRENT_SOURCE_DIR}/user
)
