# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HelloWorld CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add this line to generate the file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directory for all executables
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

# Add an executable target
add_executable(main main.cpp)
