cmake_minimum_required(VERSION 4.3)
project(lab09_materiales_iluminacion_textura LANGUAGES CXX C)

# EXTERNAL_DIR y SHARED_DIR vienen del CMakeLists.txt raiz
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

set(IMGUI_DIR "${EXTERNAL_DIR}/imgui")

add_executable(lab09_materiales_iluminacion_textura
        main.cpp
        ${EXTERNAL_DIR}/glad/src/glad.c
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/lab09-materiales-iluminacion-textura/textures
        DESTINATION ${CMAKE_BINARY_DIR}/lab09-materiales-iluminacion-textura)

target_include_directories(lab09_materiales_iluminacion_textura PRIVATE
        ${EXTERNAL_DIR}/glad/include
        ${EXTERNAL_DIR}/stb
        ${SHARED_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

target_link_libraries(lab09_materiales_iluminacion_textura PRIVATE
        glfw3
        opengl32
        gdi32
        glu32
)