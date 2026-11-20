cmake_minimum_required(VERSION 3.20)

project(OpenGLCorso LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGui")

add_executable(OpenGLCorso
    main.cpp
    glad.c
)

target_include_directories(OpenGLCorso PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include"
    "${IMGUI_DIR}"
)

target_compile_definitions(OpenGLCorso PRIVATE
    GLFW_INCLUDE_NONE
)

target_link_libraries(OpenGLCorso PRIVATE
    glfw
    OpenGL::GL
)

configure_file(
    vertex_shader_PT.glsl
    vertex_shader_PT.glsl
    COPYONLY
)

configure_file(
    fragment_shader_PT.glsl
    fragment_shader_PT.glsl
    COPYONLY
)