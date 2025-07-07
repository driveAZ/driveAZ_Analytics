#
# misc.cmake  Functions and other elements used in internal-b2v-libsm
#

cmake_minimum_required(VERSION 3.19)

function(filter_supported_flags ret lang)
    foreach(arg IN LISTS ARGN)
        check_compiler_flag(${lang} ${arg} SUPPORT)
        if(SUPPORT)
            list(APPEND output ${arg})
        endif()
    endforeach()
    set(${ret} ${output} PARENT_SCOPE)
endfunction()
