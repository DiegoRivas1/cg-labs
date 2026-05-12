cmake_minimum_required(VERSION 4.3)
project(lab04_transformaciones_jerarquicas LANGUAGES CXX C)

# SHARED_DIR y EXTERNAL_DIR vienen del CMakeLists.txt raíz
# Si abres este lab solo en CLion, descomenta y ajusta:
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

add_executable(lab04_transformaciones_jerarquicas
        main.cpp
        ${SHARED_DIR}/Poligono.cpp
        ${EXTERNAL_DIR}/glad/src/glad.c
)

file(COPY ${SHARED_DIR}/modelo/teapot.obj
        DESTINATION ${CMAKE_BINARY_DIR}/lab04-transformaciones-jerarquicas)

target_include_directories(lab04_transformaciones_jerarquicas PRIVATE
        ${EXTERNAL_DIR}/glad/include
        ${SHARED_DIR}
)

target_link_libraries(lab04_transformaciones_jerarquicas PRIVATE
        glfw3
        opengl32
        gdi32
        glu32
)