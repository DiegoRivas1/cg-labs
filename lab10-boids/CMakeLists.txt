cmake_minimum_required(VERSION 4.3)
project(lab10_boids LANGUAGES CXX C)

# EXTERNAL_DIR y SHARED_DIR vienen del CMakeLists.txt raiz
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

set(IMGUI_DIR "${EXTERNAL_DIR}/imgui")

add_executable(lab10_boids
    main.cpp
    ${EXTERNAL_DIR}/glad/src/glad.c
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

target_include_directories(lab10_boids PRIVATE
    ${EXTERNAL_DIR}/glad/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(lab10_boids PRIVATE
    glfw3
    opengl32
    gdi32
    glu32
)
