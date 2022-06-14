project(nucleus_test LANGUAGES C VERSION 0.0.1)

# Include sources
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/externals/stb/include/
    ${CMAKE_SOURCE_DIR}/externals/tinyobjloader/include/
    ${CMAKE_SOURCE_DIR}/externals/json/include/
)

# Compile and link
add_executable(${PROJECT_NAME} test.c)
target_link_libraries(${PROJECT_NAME} PRIVATE nucleus)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$(Configuration))

# Add test modules
add_subdirectory(testmodule)
add_subdirectory(test2module)

# Add run command
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)