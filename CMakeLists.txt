cmake_minimum_required(VERSION 3.9.0)

# Project
project(nucleus VERSION 0.0.1)

# Build type
set(CMAKE_BUILD_TYPE Release)

# Build directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler flags
if (MSVC)
	add_compile_options(/W2 /WX)
else (UNIX OR MINGW)
    add_compile_options(-pedantic -Wall -Wextra -Werror)
endif()

# Projects
add_subdirectory(nucleus)
add_subdirectory(examples)