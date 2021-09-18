CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus-lua LANGUAGES C VERSION 0.0.1)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

if (MSVC)
	ADD_COMPILE_OPTIONS(/W2 /WX)
else (UNIX OR MINGW)
    ADD_COMPILE_OPTIONS(-Wall)
endif()

INCLUDE_DIRECTORIES(
    ../../../extlibs/lua/include/
    ../../../
)

FILE(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

ADD_LIBRARY(
    ${PROJECT_NAME} SHARED
    ${sources}
    ${lua_sources}
)

if (MSVC)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
    )
elseif (MINGW)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        mingw32
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/lua/lib/lib-mingw-w64/liblua.a
    )
elseif (UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
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