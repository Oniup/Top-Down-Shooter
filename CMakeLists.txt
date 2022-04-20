cmake_minimum_required(VERSION 3.20)
project(topdown_shooter VERSION 0.0.1)

add_subdirectory(safire)

add_executable(${CMAKE_PROJECT_NAME} main.c)

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/vender/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/safire/include/"
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME} 
  PRIVATE safire
)
