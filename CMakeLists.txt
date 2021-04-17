cmake_minimum_required(VERSION 3.17)
project(market)

set(CMAKE_CXX_STANDARD 17)

if(NOT market_POPULATED)
    add_library(market INTERFACE)
    target_include_directories(market INTERFACE inc/)
    add_subdirectory(tests)
endif()