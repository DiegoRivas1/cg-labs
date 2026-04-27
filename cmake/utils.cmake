# utils.cmake
# Busca todas las subcarpetas que contengan un CMakeLists.txt
# y loos agrega automáticamente como subproyectos.

function(add_subprojects root_dir)
    file(GLOB children RELATIVE ${root_dir} ${root_dir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${root_dir}/${child})
            if(EXISTS ${root_dir}/${child}/CMakeLists.txt)
                message(STATUS "Agregando subproyecto: ${child}")
                add_subdirectory(${root_dir}/${child})
            endif()
        endif()
    endforeach()
endfunction()