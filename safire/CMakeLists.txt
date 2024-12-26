cmake_minimum_required(VERSION 3.20)
project(safire VERSION 0.1.3 LANGUAGES C)

set(SAFIRE_NAME safire)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/glfw)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SAFIRE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.c")
file(GLOB_RECURSE SAFIRE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.h")

add_library(
  glad STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/glad.h
  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/src/glad.c
)
target_include_directories(glad PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

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
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

add_custom_target(safire_shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  ${CMAKE_CURRENT_BINARY_DIR}/shaders
)
add_custom_target(safire_textures
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures
  ${CMAKE_CURRENT_BINARY_DIR}/textures
)
add_dependencies(${SAFIRE_NAME} safire_shaders safire_textures)
