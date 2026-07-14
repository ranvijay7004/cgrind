cmake_minimum_required(VERSION 3.20)

project(cgrind LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics audio system window)

add_subdirectory(snake)
add_subdirectory(platformer)
add_subdirectory(pong)
add_subdirectory(breakout)
add_subdirectory(space_invaders)