CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus-glfw LANGUAGES C VERSION 0.0.1)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

if (MSVC)
	ADD_COMPILE_OPTIONS(/W2 /WX)
else (UNIX OR MINGW)
    ADD_COMPILE_OPTIONS(-Wall)
endif()

ADD_DEFINITIONS(-DGLEW_STATIC -DGLEW_NO_GLU)

IF (NU_BUILD_MODULE_VULKAN)
    FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)
ENDIF()

INCLUDE_DIRECTORIES(
    ../../../externals/glfw/include/
    ../../../externals/glew/include/
    ../../../
    IF (NU_BUILD_MODULE_VULKAN)
        ${Vulkan_INCLUDE_DIRS}
    ENDIF()
)

FILE(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

ADD_LIBRARY(
    ${PROJECT_NAME} SHARED
    ../../../externals/glew/src/glew.c
    ${sources}
)

FIND_PACKAGE(OpenGL)

if (MSVC)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        "${OPENGL_gl_LIBRARY}"
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/glfw/lib/lib-vc2019/glfw3.lib
    )
elseif (MINGW)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        "${OPENGL_gl_LIBRARY}" 
        mingw32
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/glfw/lib/lib-mingw-w64/libglfw3.a
    )
elseif (UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        "${OPENGL_gl_LIBRARY}"
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/glfw/lib/lib-unix/libglfw3.a
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