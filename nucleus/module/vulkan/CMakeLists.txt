CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

PROJECT(nucleus-vulkan LANGUAGES C CXX VERSION 0.0.1)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

if (MSVC)
	ADD_COMPILE_OPTIONS(/W2 /WX)
else (UNIX OR MINGW)
    ADD_COMPILE_OPTIONS(-Wall)
endif()

FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

INCLUDE_DIRECTORIES(
    ../../../
    ../../../extlibs/vma/include/
    ../../../extlibs/shaderc/include/
    ${Vulkan_INCLUDE_DIRS}
)

FILE(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

ADD_LIBRARY(
    ${PROJECT_NAME} SHARED
    vma/vma_allocator.cpp
    ${sources}
)

if (MSVC)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        ${Vulkan_LIBRARIES}
    )
elseif (MINGW)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        mingw32
        ${Vulkan_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/shaderc/lib/lib-mingw-w64/libshaderc_combined.a
    )
elseif (UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        nucleus
        ${Vulkan_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../extlibs/shaderc/lib/lib-unix/libshaderc_combined.a
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