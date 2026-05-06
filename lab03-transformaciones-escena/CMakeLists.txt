cmake_minimum_required(VERSION 4.3)
project(lab03 LANGUAGES CXX C)

# EXTERNAL_DIR viene definido desde el CMakeLists.txt raíz de cg-labs.
# Si abres lab02 solo en CLion, descomenta y ajusta:
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")

add_executable(lab03
        ${EXTERNAL_DIR}/glad/src/glad.c
        main.cpp
        Poligono.cpp
        Poligono.h
        OBJLoader.h
        Primitives.h
)

file(COPY ${CMAKE_SOURCE_DIR}/lab03-transformaciones-escena/modelo/teapot.obj
        DESTINATION ${CMAKE_BINARY_DIR}/lab03)

target_include_directories(lab03 PRIVATE
        ${EXTERNAL_DIR}/glad/include
)

target_link_libraries(lab03 PRIVATE
        glfw3
        opengl32
        gdi32
        glu32
)