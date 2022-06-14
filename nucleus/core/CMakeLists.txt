project(nucleus VERSION 0.0.1 LANGUAGES C DESCRIPTION "The nucleus core library")

# Source files
file(GLOB_RECURSE sources_ini ${CMAKE_SOURCE_DIR}/externals/ini/include/*.c)
file(GLOB_RECURSE sources_cjson ${CMAKE_SOURCE_DIR}/externals/cJSON/include/*.c)
file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Compile and link
add_library(
    ${PROJECT_NAME} SHARED
    ${sources_ini}
    ${sources_cjson}
    ${sources}
)

if (MSVC)

elseif (MINGW)
    target_link_libraries(${PROJECT_NAME}
        -static
        -static-libgcc
        -static-libstdc++
        pthread
        mingw32
    )
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME}
        pthread
        dl
        m
    )
endif()