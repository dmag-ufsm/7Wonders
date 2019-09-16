cmake_minimum_required(VERSION 3.5)

project(7Wonders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
