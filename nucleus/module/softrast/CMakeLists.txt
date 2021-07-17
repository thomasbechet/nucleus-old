CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus-softrast LANGUAGES CXX VERSION 0.0.1)

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE Release)

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O2")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	ADD_COMPILE_OPTIONS(/W2 /WX)
else (UNIX OR MINGW)
    ADD_COMPILE_OPTIONS(-Wall)
endif()

FIND_PACKAGE(OpenMP)
if (OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

INCLUDE_DIRECTORIES(
    ../../../
    ../../../extlibs/stb/include/
    ../../../extlibs/freetype/include/
)

FILE(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ADD_LIBRARY(
    ${PROJECT_NAME} SHARED
    ${sources}
)

if (MSVC)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/freetype/lib/lib-vc2019/freetype.lib
    )
elseif (MINGW)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus 
        mingw32
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/freetype/lib/lib-mingw-w64/libfreetype.a
    )
elseif (UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/freetype/lib/lib-unix/libfreetype.a
    )
endif()

if (MSVC OR MINGW)
    INSTALL(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/bin/engine/module/
    )
elseif (UNIX)
    INSTALL(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/bin/engine/module/
    )
endif()