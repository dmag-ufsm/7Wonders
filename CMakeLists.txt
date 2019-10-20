cmake_minimum_required(VERSION 3.5)

project(7Wonders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# if (NOT BUILD_SHARED_LIBS)
#     add_definitions(-DAMQP_STATIC)
# endif()

include_directories(include)
# link_directories(lib)

set(SOURCES
    src/card.cpp
    src/player.cpp
    src/wonder.cpp
	)

set(HEADERS
    include/card.h
    include/player.h
    include/wonder.h
	)

add_executable(7Wonders main.cpp ${SOURCES} ${HEADERS})
# target_link_libraries(7Wonders librabbitmq.so)
