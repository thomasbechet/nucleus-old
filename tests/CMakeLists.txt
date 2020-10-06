CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus_test LANGUAGES C VERSION 0.0.1)

INCLUDE_DIRECTORIES(
    ../
    ../extlibs/stb/include/
    ../extlibs/tinyobjloader/include/
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    main.c
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME} PRIVATE
    nucleus
)

INSTALL(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin"
)