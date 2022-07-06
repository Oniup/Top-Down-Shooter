cmake_minimum_required(VERSION 3.20)
project(shooter_game VERSION 0.0.7 LANGUAGES C)

add_subdirectory(safire)

set(GAME_NAME "topdown_shooter")

# game and its scenes source code
file(GLOB_RECURSE GAME_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${GAME_NAME}/src/*.c")
file(GLOB_RECURSE GAME_SCENE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${GAME_NAME}/scenes/src/*.c")

add_executable(${CMAKE_PROJECT_NAME} main.c ${GAME_SOURCES} ${GAME_SCENE_SOURCES})

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  # safire game engine
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/dependencies/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/include/"
  # the game and its scenes
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${GAME_NAME}/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${GAME_NAME}/include/"
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME} 
  PRIVATE safire
)

add_custom_target(shooter_game_art
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/topdown_shooter/art
  ${CMAKE_CURRENT_BINARY_DIR}/art
)
add_dependencies(${CMAKE_PROJECT_NAME} shooter_game_art)
