cmake_minimum_required(VERSION 3.20)
project(topdown_shooter VERSION 0.0.1 LANGUAGES C)

add_subdirectory(safire)

# game and its scenes source code
file(GLOB_RECURSE GAME_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_PROJECT_NAME}/src/*.c")
file(GLOB_RECURSE GAME_SCENE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_PROJECT_NAME}/scenes/src/*.c")

add_executable(${CMAKE_PROJECT_NAME} main.c ${GAME_SOURCES} ${GAME_SCENE_SOURCES})

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  # safire game engine
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/dependencies/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/include/"
  # the game and its scenes
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/include/"
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME} 
  PRIVATE safire
)
