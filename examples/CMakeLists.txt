CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus_test LANGUAGES C VERSION 0.0.1)

ADD_COMPILE_DEFINITIONS(NU_DEBUG)
SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(
    ../
    ../externals/stb/include/
    ../externals/tinyobjloader/include/
    ../externals/json/include/
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    #main.c
    #containers_test.c
    test.c
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME} PRIVATE
    nucleus
)

INSTALL(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin"
)