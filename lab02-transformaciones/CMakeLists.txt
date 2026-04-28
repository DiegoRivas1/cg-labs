cmake_minimum_required(VERSION 4.3)
project(lab02_transformaciones LANGUAGES CXX C)

# EXTERNAL_DIR viene definido desde el CMakeLists.txt raíz de cg-labs.
# Si abres lab02 solo en CLion, descomenta y ajusta:
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")

add_executable(lab02_transformaciones
        main.cpp
        shapes.cpp
        ${EXTERNAL_DIR}/glad/src/glad.c
)

target_include_directories(lab02_transformaciones PRIVATE
        ${EXTERNAL_DIR}/glad/include
)

target_link_libraries(lab02_transformaciones PRIVATE
        glfw3
        opengl32
        gdi32
)