cmake_minimum_required(VERSION 4.3)
project(lab05_proyecciones_camara LANGUAGES CXX C)

# EXTERNAL_DIR y SHARED_DIR vienen del CMakeLists.txt raíz
# Si abres este lab solo en CLion, descomenta y ajusta:
# set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/../../external")
# set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/../../shared")

add_executable(lab05_proyecciones_camara
        main.cpp
        ${SHARED_DIR}/Poligono.cpp
        ${EXTERNAL_DIR}/glad/src/glad.c
)

target_include_directories(lab05_proyecciones_camara PRIVATE
        ${EXTERNAL_DIR}/glad/include
        ${SHARED_DIR}
)

target_link_libraries(lab05_proyecciones_camara PRIVATE
        glfw3
        opengl32
        gdi32
        glu32
)
