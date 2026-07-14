# space_invaders — self-contained game target.
#
# Can be built as part of the repo (added via the root add_subdirectory) or
# on its own:  cmake -S . -B build && cmake --build build

cmake_minimum_required(VERSION 3.20)

project(space_invaders LANGUAGES CXX)

# When built standalone the SFML targets don't exist yet; find them here.
# When built from the repo root the root already provided them, so we skip.
if(NOT TARGET sfml-graphics)
    find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio)
endif()

add_executable(space_invaders src/main.cpp)

target_compile_features(space_invaders PRIVATE cxx_std_23)
target_compile_options(space_invaders PRIVATE -Wall -Wextra)

target_link_libraries(space_invaders PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio)

# Copy this game's assets next to the executable so it runs straight from the
# build directory with no manual copying of textures, sounds or fonts.
add_custom_command(TARGET space_invaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:space_invaders>/assets"
    COMMENT "Copying space_invaders assets to build directory")
