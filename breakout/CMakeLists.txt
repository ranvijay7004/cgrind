# breakout — self-contained game target.
#
# Can be built as part of the repo (added via the root add_subdirectory) or
# on its own:  cmake -S . -B build && cmake --build build

cmake_minimum_required(VERSION 3.20)

project(breakout LANGUAGES CXX)

# When built standalone the SFML targets don't exist yet; find them here.
# When built from the repo root the root already provided them, so we skip.
if(NOT TARGET sfml-graphics)
    find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio)
endif()

add_executable(breakout src/main.cpp)

target_compile_features(breakout PRIVATE cxx_std_23)
target_compile_options(breakout PRIVATE -Wall -Wextra)

target_link_libraries(breakout PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio)

# Copy this game's assets next to the executable so it runs straight from the
# build directory with no manual copying of textures, sounds or fonts.
add_custom_command(TARGET breakout POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:breakout>/assets"
    COMMENT "Copying breakout assets to build directory")
