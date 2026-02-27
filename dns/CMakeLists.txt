cmake_minimum_required(VERSION 3.10)
# Define the project as C
project(DNS C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# Flags for C debugging
add_compile_options(-fsanitize=address -g)
add_link_options(-fsanitize=address)

# Ensure the bin directory exists
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

# If your file is main.c (lowercase), this works perfectly
add_executable(main main.c)