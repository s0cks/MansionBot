cmake_minimum_required(VERSION 3.2)
project(MansionBot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
include_directories(/usr/include/curl)

set(SOURCE_FILES main.cpp)
add_executable(MansionBot ${SOURCE_FILES} include/http_client.h include/database.h include/twitch.h)
target_link_libraries(MansionBot curl)
target_link_libraries(MansionBot sqlite3)