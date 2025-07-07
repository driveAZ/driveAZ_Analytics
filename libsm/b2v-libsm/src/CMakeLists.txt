#
# CMakeLists for src/
#

set(LIBSM_HEADERS
        j2735-defines.h
        j2945-defines.h
        libsm-error.h
        libsm-pathHistory.h
        libsm-per.h
        libsm.h
        pathPrediction.h
        libsm-SPAT.h
        libsm-TIM.h
	    octet-helpers.h
)
set(LIBSM_SRCS
        libsm-error.c
        libsm-pathHistory.c
        libsm-per.c
        libsm.c
        pathPrediction.c
        libsm-SPAT.c
        libsm-TIM.c
	    octet-helpers.c
)

add_library(libsm SHARED ${LIBSM_SRCS} ${LIBSM_HEADERS})
target_compile_options(libsm PRIVATE
        -Wall
        -Wextra
)
add_subdirectory(j2735)
set(j2735_EMIT_DEBUG OFF CACHE BOOL "Switch on asn debug")
add_subdirectory(j2540)

target_include_directories(libsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(libsm PRIVATE m)
target_link_libraries(libsm PUBLIC j2735 j2540)
