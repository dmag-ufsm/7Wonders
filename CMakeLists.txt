cmake_minimum_required(VERSION 3.5)

project(7Wonders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)

set(SOURCES
    src/card.cpp
    src/player.cpp
    src/deck.cpp
    src/messager.cpp)

set(HEADERS
    include/card.h
    include/player.h
    include/deck.h
    include/messager.h)

add_executable(7Wonders main.cpp ${SOURCES} ${HEADERS})
add_executable(tests src/tests.cpp ${SOURCES} ${HEADERS})
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)