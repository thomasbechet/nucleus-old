project(nucleus-package VERSION 0.0.1)

include_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/externals/cglm/include/
    ${CMAKE_SOURCE_DIR}/externals/ini/include/
    ${CMAKE_SOURCE_DIR}/externals/cJSON/include/
)

option(NU_DEBUG               "Debug"                   ON)
option(NU_DEBUG_MEMORY        "Debug memory"            ON)

option(NU_BUILD_CORE          "Build nucleus-core"      ON)

option(NU_BUILD_MODULE_VULKAN "Build nucleus-vulkan"    OFF)
option(NU_BUILD_MODULE_GLFW   "Build nucleus-glfw"      OFF)
option(NU_BUILD_MODULE_UTILS  "Build nucleus-utils"     OFF)
option(NU_BUILD_MODULE_LUA    "Build nucleus-lua"       OFF)
option(NU_BUILD_MODULE_ECS    "Build nucleus-ecs"       OFF)

# Add compile definitions
if (NU_DEBUG)
    add_compile_definitions(NU_DEBUG)
endif()
if (NU_DEBUG_MEMORY)
    add_compile_definitions(NU_DEBUG_MEMORY)
endif()

# Add subdirectories
if (NU_BUILD_CORE)
    add_subdirectory(core)
    add_compile_definitions(NU_BUILD_CORE)
endif()

# # Add subdirectories
# if (NU_BUILD_MODULE_VULKAN)
#     add_subdirectory(module/vulkan)
#     add_compile_definitions(NU_BUILD_MODULE_VULKAN)
# endif()
# if (NU_BUILD_MODULE_GLFW)
#     add_subdirectory(module/glfw)
#     add_compile_definitions(NU_BUILD_MODULE_GLFW)
# endif()
# if (NU_BUILD_MODULE_UTILS)
#     add_subdirectory(module/utils)
#     add_compile_definitions(NU_BUILD_MODULE_UTILS)
# endif()
# if (NU_BUILD_MODULE_LUA)
#     add_subdirectory(module/lua)
#     add_compile_definitions(NU_BUILD_MODULE_LUA)
# endif()
# if (NU_BUILD_MODULE_ECS)
#     add_subdirectory(module/ecs)
#     add_compile_definitions(NU_BUILD_MODULE_ECS)
# endif()