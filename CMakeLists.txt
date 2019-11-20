cmake_minimum_required(VERSION 3.5)

project(7Wonders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include_directories(include)

set(SOURCES
    src/card.cpp
	src/deck.cpp
	src/player.cpp
    src/wonder.cpp
	src/filer.cpp
	)

set(HEADERS
    include/card.h
	include/deck.h
	include/player.h
	include/resources.h
    include/wonder.h
	include/filer.h
	include/nlohmann/json.hpp
	)

add_executable(7Wonders main.cpp ${SOURCES} ${HEADERS})
