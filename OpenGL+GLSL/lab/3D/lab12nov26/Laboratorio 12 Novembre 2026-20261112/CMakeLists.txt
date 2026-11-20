cmake_minimum_required(VERSION 3.16)
project(Laboratorio12Nov LANGUAGES C CXX)

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

set(DEPS "${CMAKE_CURRENT_SOURCE_DIR}/dependencies_GL_GLFW")
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

# ==========================================================
# Target 1: EsempiodiAnimazionesuCerchio
# ==========================================================
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/EsempiodiAnimazionesuCerchio")
set(CERCHIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/EsempiodiAnimazionesuCerchio")
file(GLOB CERCHIO_SOURCES "${CERCHIO_DIR}/*.cpp")

add_executable(EsempiodiAnimazionesuCerchio ${CERCHIO_SOURCES})
set_target_properties(EsempiodiAnimazionesuCerchio PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/EsempiodiAnimazionesuCerchio"
)

target_include_directories(EsempiodiAnimazionesuCerchio PRIVATE
  ${CERCHIO_DIR}
  ${DEPS}/include
  ${IMGUI}
)

target_link_libraries(EsempiodiAnimazionesuCerchio PRIVATE
  glad
  imgui
  glfw
  ${OPENGL_LIBRARIES}
  ${GLM_TARGET}
)

if(APPLE)
  target_link_libraries(EsempiodiAnimazionesuCerchio PRIVATE
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
endif()

# Copia cartella shaders accanto all'eseguibile EsempiodiAnimazionesuCerchio
add_custom_command(TARGET EsempiodiAnimazionesuCerchio POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CERCHIO_DIR}/shaders" "$<TARGET_FILE_DIR:EsempiodiAnimazionesuCerchio>/shaders"
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:EsempiodiAnimazionesuCerchio>" "${CERCHIO_DIR}/EsempiodiAnimazionesuCerchio"
)

# ==========================================================
# Target 2: ProgettoOpengl_3d_Starter
# ==========================================================
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/ProgettoOpengl_3d_Starter")
set(STARTER_3D_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ProgettoOpengl_3d_Starter")
file(GLOB STARTER_3D_SOURCES "${STARTER_3D_DIR}/*.cpp")

add_executable(ProgettoOpengl_3d_Starter ${STARTER_3D_SOURCES})
set_target_properties(ProgettoOpengl_3d_Starter PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ProgettoOpengl_3d_Starter"
)

target_include_directories(ProgettoOpengl_3d_Starter PRIVATE
  ${STARTER_3D_DIR}
  ${DEPS}/include
  ${IMGUI}
)

target_link_libraries(ProgettoOpengl_3d_Starter PRIVATE
  glad
  imgui
  glfw
  ${OPENGL_LIBRARIES}
  ${GLM_TARGET}
)

if(APPLE)
  target_link_libraries(ProgettoOpengl_3d_Starter PRIVATE
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
endif()

# Copia cartella shaders accanto all'eseguibile ProgettoOpengl_3d_Starter
add_custom_command(TARGET ProgettoOpengl_3d_Starter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${STARTER_3D_DIR}/shaders" "$<TARGET_FILE_DIR:ProgettoOpengl_3d_Starter>/shaders"
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:ProgettoOpengl_3d_Starter>" "${STARTER_3D_DIR}/ProgettoOpengl_3d_Starter"
)

# ==========================================================
# Target 3: ProgettoOpengl_3d_DueCubi
# ==========================================================
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/ProgettoOpengl_3d_DueCubi")
set(DUE_CUBI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ProgettoOpengl_3d_DueCubi")
file(GLOB DUE_CUBI_SOURCES "${DUE_CUBI_DIR}/*.cpp")

add_executable(ProgettoOpengl_3d_DueCubi ${DUE_CUBI_SOURCES})
set_target_properties(ProgettoOpengl_3d_DueCubi PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ProgettoOpengl_3d_DueCubi"
)

target_include_directories(ProgettoOpengl_3d_DueCubi PRIVATE
  ${DUE_CUBI_DIR}
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

# Copia cartella shaders accanto all'eseguibile ProgettoOpengl_3d_DueCubi
add_custom_command(TARGET ProgettoOpengl_3d_DueCubi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${DUE_CUBI_DIR}/shaders" "$<TARGET_FILE_DIR:ProgettoOpengl_3d_DueCubi>/shaders"
  COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:ProgettoOpengl_3d_DueCubi>" "${DUE_CUBI_DIR}/ProgettoOpengl_3d_DueCubi"
)

