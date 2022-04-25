cmake_minimum_required(VERSION 3.20)
project(safire VERSION 0.0.5 LANGUAGES C)

set(SAFIRE_NAME safire)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vender/glfw)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SAFIRE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.c")
file(GLOB_RECURSE SAFIRE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/safire*.h")

add_library(
  glad STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/vender/glad/glad.h
  ${CMAKE_CURRENT_SOURCE_DIR}/vender/src/glad.c
)
target_include_directories(glad PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vender")

add_library(
  ${SAFIRE_NAME} STATIC
  ${SAFIRE_HEADERS}
  ${SAFIRE_SOURCES}
)
target_link_libraries(
  ${SAFIRE_NAME}
  PRIVATE glad
  PRIVATE glfw
  PRIVATE OpenGL::GL
)
target_include_directories(
  ${SAFIRE_NAME}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vender"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)