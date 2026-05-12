cmake_minimum_required(VERSION 4.0.1)
project(cg_labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ruta a external compartida para todos los labs
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external" CACHE PATH "Ruta a external/")
set(SHARED_DIR   "${CMAKE_SOURCE_DIR}/shared"   CACHE PATH "Ruta a shared/")
# Agrega automáticamente cada lab que tenga su propio CMakeLists.txt
include(cmake/utils.cmake)
add_subprojects(${CMAKE_SOURCE_DIR})