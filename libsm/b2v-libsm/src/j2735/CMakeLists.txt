#
# CMakeLists.txt for j2735 library files
# GENERATED FILE
#

include(${PROJECT_SOURCE_DIR}/tooling/misc.cmake)
include(${PROJECT_SOURCE_DIR}/tooling/colours.cmake)

include(CMakeLists.files.txt)
include(CheckCompilerFlag)


# you just do "cmake -DJ2735_EMIT_DEBUG=ON .." to set
option(J2735_EMIT_DEBUG "enable ASN_EMIT_DEBUG for J2735" OFF)

add_library(j2735 SHARED ${J2735_SRCS} ${J2735_HEADERS})

filter_supported_flags(J2735_CFLAGS "C"
        -Wall
        -Wextra
        -Werror
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-parentheses-equality
        -Wno-unused-but-set-variable
        -Wno-array-bounds
)
target_compile_options(j2735 PRIVATE ${J2735_CFLAGS})

if(J2735_EMIT_DEBUG)
    message("${ttyCy}-- Using ASN_EMIT_DEBUG for j2735${ttyN}")
    target_compile_options(j2735 PRIVATE -DASN_EMIT_DEBUG=1)
else()
    target_compile_options(j2735 PRIVATE -DASN_EMIT_DEBUG=0)
endif()


target_include_directories(j2735 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
