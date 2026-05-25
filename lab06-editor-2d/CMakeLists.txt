cmake_minimum_required(VERSION 3.20)
project(lab06_editor_2d LANGUAGES CXX C)

# EXTERNAL_DIR y SHARED_DIR vienen del CMakeLists.txt raiz
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

set(IMGUI_DIR "${EXTERNAL_DIR}/imgui")

add_executable(lab06_editor_2d
    main.cpp
    ${EXTERNAL_DIR}/glad/src/glad.c
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

target_include_directories(lab06_editor_2d PRIVATE
    ${EXTERNAL_DIR}/glad/include
    ${SHARED_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(lab06_editor_2d PRIVATE
    glfw3
    opengl32
    gdi32
)
