cmake_minimum_required(VERSION 3.5)

project(7Wonders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(include)

set(SOURCES
    main.cpp
    src/card.cpp
	src/player.cpp
    src/wonder.cpp
	src/filer.cpp
	)

set(HEADERS
    include/card.h
	include/player.h
	include/resources.h
    include/wonder.h
	include/filer.h
	include/nlohmann/json.hpp
	)

add_executable(7Wonders ${SOURCES} ${HEADERS})

file(MAKE_DIRECTORY ./build/io ./io)
