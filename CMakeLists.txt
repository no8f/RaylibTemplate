cmake_minimum_required(VERSION 4.0)

project(template)

# Provide compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(export-compile-commands ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  COMMENT "Export compile_commands.json to source root"
)

# OPTIONAL support variable framerate
# add_compile_definitions(SUPPORT_CUSTOM_FRAME_CONTROL=1)

include(cmake/CPM.cmake)

CPMAddPackage("gh:raysan5/raylib#5.5")

add_subdirectory(src)
