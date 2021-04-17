cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        )
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${json_SOURCE_DIR})
endif()

add_executable(test_market test_market.cc)

target_link_libraries(test_market gtest gtest_main)
target_link_libraries(test_market pthread)
target_link_libraries(test_market common)
target_link_libraries(test_market lob)
target_link_libraries(test_market nlohmann_json::nlohmann_json)
target_link_libraries(test_market market)
