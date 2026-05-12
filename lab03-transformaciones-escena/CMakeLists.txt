cmake_minimum_required(VERSION 4.3)
project(lab03_transformaciones_escena LANGUAGES CXX C)

# SHARED_DIR y EXTERNAL_DIR vienen del CMakeLists.txt raíz
# Si abres este lab solo en CLion, descomenta y ajusta:
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

add_executable(lab03_transformaciones_escena
        ${EXTERNAL_DIR}/glad/src/glad.c
        main.cpp
        ${SHARED_DIR}/Poligono.cpp
        #../shared/Poligono.cpp
        #../shared/Poligono.h
        #../shared/OBJLoader.h
        #../shared/Primitives.h
)

file(COPY ${SHARED_DIR}/modelo/teapot.obj
        DESTINATION ${CMAKE_BINARY_DIR}/lab03-transformaciones-escena)

target_include_directories(lab03_transformaciones_escena PRIVATE
        ${EXTERNAL_DIR}/glad/include
        ${SHARED_DIR}
)

target_link_libraries(lab03_transformaciones_escena PRIVATE
        glfw3
        opengl32
        gdi32
        glu32
)