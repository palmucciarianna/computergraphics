cmake_minimum_required(VERSION 3.16)
project(ProgettoOpengl_3d_DueCubi LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH /opt/homebrew /usr/local)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  find_path(GLM_INCLUDE_DIR glm/glm.hpp HINTS /opt/homebrew/include /usr/local/include /opt/homebrew/Cellar/glm/1.0.3/include)
  if(NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "GLM non trovato. Installalo con: brew install glm")
  endif()
  add_library(glm INTERFACE)
  target_include_directories(glm INTERFACE "${GLM_INCLUDE_DIR}")
  set(GLM_TARGET glm)
else()
  set(GLM_TARGET glm::glm)
endif()

set(DEPS "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies_GL_GLFW")
set(IMGUI "${DEPS}/ImGui")

# Libreria GLAD
add_library(glad STATIC "${DEPS}/glad.c")
target_include_directories(glad PUBLIC "${DEPS}/include")
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

# Libreria ImGui
add_library(imgui STATIC
  "${IMGUI}/imgui.cpp"
  "${IMGUI}/imgui_draw.cpp"
  "${IMGUI}/imgui_tables.cpp"
  "${IMGUI}/imgui_widgets.cpp"
  "${IMGUI}/imgui_impl_glfw.cpp"
  "${IMGUI}/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC "${IMGUI}" "${DEPS}/include")
target_compile_definitions(imgui PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(ProgettoOpengl_3d_DueCubi ${SOURCES})

target_include_directories(ProgettoOpengl_3d_DueCubi PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DEPS}/include
  ${IMGUI}
)

target_link_libraries(ProgettoOpengl_3d_DueCubi PRIVATE
  glad
  imgui
  glfw
  ${OPENGL_LIBRARIES}
  ${GLM_TARGET}
)

if(APPLE)
  target_link_libraries(ProgettoOpengl_3d_DueCubi PRIVATE
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
endif()

# Copia la cartella shaders accanto all'eseguibile e nella cartella del progetto
add_custom_command(TARGET ProgettoOpengl_3d_DueCubi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:ProgettoOpengl_3d_DueCubi>/shaders"
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:ProgettoOpengl_3d_DueCubi>" "${CMAKE_CURRENT_SOURCE_DIR}/ProgettoOpengl_3d_DueCubi"
)
