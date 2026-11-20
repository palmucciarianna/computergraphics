cmake_minimum_required(VERSION 3.20)
project(CannoneSoloBrew LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(imgui STATIC
    ImGui/imgui.cpp
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    ImGui/imgui_impl_glfw.cpp
    ImGui/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGui"
)
target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

add_executable(cannone
    main_studenti.cpp
    Geometry.cpp
    Gui.cpp
    Input.cpp
    Renderer.cpp
    Scene.cpp
    Shader.cpp
    ShaderMaker.cpp
    ShapeGenerator.cpp
)

target_compile_definitions(cannone PRIVATE
    GL_SILENCE_DEPRECATION
    GLFW_INCLUDE_NONE
)

target_link_libraries(cannone PRIVATE
    imgui
    glfw
    glm::glm-header-only
    OpenGL::GL
)

add_custom_command(TARGET cannone POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:cannone>/shader"
)
