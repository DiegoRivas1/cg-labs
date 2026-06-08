cmake_minimum_required(VERSION 4.3)
project(lab07_editor_3d LANGUAGES CXX C)

# EXTERNAL_DIR y SHARED_DIR vienen del CMakeLists.txt raiz
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

set(IMGUI_DIR "${EXTERNAL_DIR}/imgui")

add_executable(lab07_editor_3d
    main.cpp
    ${SHARED_DIR}/Poligono.cpp
    ${EXTERNAL_DIR}/glad/src/glad.c
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

file(COPY ${SHARED_DIR}/modelo/teapot.obj
     DESTINATION ${CMAKE_BINARY_DIR}/lab07-editor-3d)

target_include_directories(lab07_editor_3d PRIVATE
    ${EXTERNAL_DIR}/glad/include
    ${SHARED_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(lab07_editor_3d PRIVATE
    glfw3
    opengl32
    gdi32
    glu32
)
