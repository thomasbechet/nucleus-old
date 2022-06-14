project(testmodule LANGUAGES C VERSION 0.0.1)

# Source files
file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Compile and link
add_library(${PROJECT_NAME} SHARED ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE nucleus)