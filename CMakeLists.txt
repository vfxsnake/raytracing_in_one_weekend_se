cmake_minimum_required(VERSION 3.25)
project(RayTracingInOneWeekendSE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Warning flags ─────────────────────────────────────────────────────────────
# Applied per target (never globally) so FetchContent dependencies stay quiet.
# Warnings are errors: a missing return or a narrowing conversion stops the build
# instead of reaching review.
set(PROJECT_WARNING_FLAGS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
)

enable_testing()

# ── Vulkan SDK ─────────────────────────────────────────────────────────────────
find_package(Vulkan REQUIRED)

# ── FetchContent dependencies ──────────────────────────────────────────────────
# SYSTEM: their headers are treated as system headers, so -Werror / /WX on our
# targets never fires on third-party code.
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    SYSTEM
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    VulkanHpp
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Hpp.git
    GIT_TAG        main
    SYSTEM
)
FetchContent_MakeAvailable(VulkanHpp)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
    SYSTEM
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.93.0-docking
)
FetchContent_MakeAvailable(imgui)


# ── Engine (read-only submodule) ──────────────────────────────────────────────
# Built as its own library without PROJECT_WARNING_FLAGS: warnings in the engine
# cannot be fixed from this repo (see the submodule rule in CLAUDE.md).
set(ENGINE_SRC ${CMAKE_SOURCE_DIR}/engine/vk_tutorial_se/src)

add_library(SnakeEngine STATIC
    ${ENGINE_SRC}/core/VulkanContext.cpp
    ${ENGINE_SRC}/core/SwapChain.cpp
)

target_include_directories(SnakeEngine SYSTEM PUBLIC
    ${ENGINE_SRC}
)

target_link_libraries(SnakeEngine PUBLIC
    Vulkan::Vulkan
    Vulkan::Headers
    glfw
)

target_compile_definitions(SnakeEngine PUBLIC
    VULKAN_HPP_NO_CONSTRUCTORS
)


# ── Dear ImGui ────────────────────────────────────────────────────────────────
add_library(ImGui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(ImGui SYSTEM PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(ImGui PUBLIC
    Vulkan::Vulkan
    glfw
)


# ── Executable ────────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Application.cpp
    src/framebuffer/Framebuffer.cpp
    src/display/DisplayPipeline.cpp
    src/display/GraphicsPipeline.cpp
    src/ui/imGuiLayer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SnakeEngine
    ImGui
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${PROJECT_WARNING_FLAGS}
)

# ── Shaders ───────────────────────────────────────────────────────────────────
if(WIN32)
    set(SLANG_C $ENV{VULKAN_SDK}/Bin/slangc.exe)
else()
    set(SLANG_C $ENV{VULKAN_SDK}/bin/slangc)
endif()

set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_SOURCES ${SHADERS_DIR}/display.slang)
set(DISPLAY_ENTRY_POINTS -entry vertMain -entry fragMain)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/shaders/display.spv
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${SLANG_C} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${DISPLAY_ENTRY_POINTS} -o ${CMAKE_BINARY_DIR}/shaders/display.spv
    WORKING_DIRECTORY ${SHADERS_DIR}
    DEPENDS ${SHADER_SOURCES}
    COMMENT "compiling slang shaders"
    VERBATIM
)

add_custom_target(Shaders DEPENDS
    ${CMAKE_BINARY_DIR}/shaders/display.spv
)
add_dependencies(${PROJECT_NAME} Shaders)

# ── Assets ────────────────────────────────────────────────────────────────────
# Copies assets/ into the build directory on every build, so a clean build
# never needs files copied by hand. Paths in code are relative: "assets/...".
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
    COMMENT "copying assets"
)
add_dependencies(${PROJECT_NAME} CopyAssets)

# ── Tests ─────────────────────────────────────────────────────────────────────
add_subdirectory(tests)
