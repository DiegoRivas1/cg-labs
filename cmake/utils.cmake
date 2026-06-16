# utils.cmake
# Busca todas las subcarpetas que contengan un CMakeLists.txt
# y loos agrega automáticamente como subproyectos.

function(add_subprojects root_dir)
    set(excluded_dirs build cmake-build-debug external imgui-1.92.8 cg-labs)
    file(GLOB children RELATIVE "${root_dir}" "${root_dir}/*")
    foreach(child ${children})
        list(FIND excluded_dirs "${child}" excluded_index)
        if(excluded_index EQUAL -1 AND IS_DIRECTORY "${root_dir}/${child}")
            if(EXISTS "${root_dir}/${child}/CMakeLists.txt")
                message(STATUS "Agregando subproyecto: ${child}")
                add_subdirectory("${root_dir}/${child}" "${CMAKE_BINARY_DIR}/${child}")
            endif()
        endif()
    endforeach()
endfunction()