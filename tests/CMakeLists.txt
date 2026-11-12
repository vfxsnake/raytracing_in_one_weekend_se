list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# add_project_test(<target_name> [extra sources...])
#
# The test source is derived from the target name (test_vec3 -> test_vec3.cpp).
# Any extra arguments are additional .cpp files the test needs to link.
function(add_project_test target_name)
    add_executable(${target_name}
        ${target_name}.cpp
        ${ARGN}
    )

    target_include_directories(${target_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    )

    target_link_libraries(${target_name} PRIVATE
        Catch2::Catch2WithMain
    )

    target_compile_options(${target_name} PRIVATE
        ${PROJECT_WARNING_FLAGS}
    )

    catch_discover_tests(${target_name})
endfunction()

add_project_test(test_framebuffer ${CMAKE_SOURCE_DIR}/src/framebuffer/Framebuffer.cpp)
