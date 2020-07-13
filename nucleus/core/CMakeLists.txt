CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus-core LANGUAGES C VERSION 0.0.1)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
SET(CMAKE_EXE_LINKER_FLAGS " -static")

FIND_PACKAGE(OpenMP)
if (OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

INCLUDE_DIRECTORIES(
    ../../extlibs/cglm/include/
    ../../extlibs/ini/include/
)

FILE(GLOB_RECURSE sources_cglm ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/cglm/src/*.c)
FILE(GLOB_RECURSE sources_ini ${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/ini/include/*.c)
FILE(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

ADD_LIBRARY(
    ${PROJECT_NAME} SHARED
    ${sources_cglm}
    ${sources_ini}
    ${sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    -static
    -static-libgcc
    -static-libstdc++
    pthread
    mingw32
)

INSTALL(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/
)
