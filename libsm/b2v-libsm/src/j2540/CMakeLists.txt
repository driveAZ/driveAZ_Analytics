#
# CMakeLists.txt for j2540 library files
# GENERATED FILE
#

include(${PROJECT_SOURCE_DIR}/tooling/misc.cmake)
include(${PROJECT_SOURCE_DIR}/tooling/colours.cmake)

include(CMakeLists.files.txt)
include(CheckCompilerFlag)


# you just do "cmake -DJ2735_EMIT_DEBUG=ON .." to set
option(J2540_EMIT_DEBUG "enable ASN_EMIT_DEBUG for J2540" OFF)

add_library(j2540 SHARED ${J2540_SRCS} ${J2540_HEADERS})

filter_supported_flags(J2540_CFLAGS "C"
        -Wall
        -Wextra
        -Werror
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-parentheses-equality
        -Wno-unused-but-set-variable
        -Wno-array-bounds
)
target_compile_options(j2540 PRIVATE ${J2540_CFLAGS})

if(J2540_EMIT_DEBUG)
    message("${ttyCy}-- Using ASN_EMIT_DEBUG for j2540${ttyN}")
    target_compile_options(j2540 PRIVATE -DASN_EMIT_DEBUG=1)
else()
    target_compile_options(j2540 PRIVATE -DASN_EMIT_DEBUG=0)
endif()


target_include_directories(j2540 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
