#
# CMakelists.txt for internal-b2v-libsm
#

cmake_minimum_required(VERSION 3.21)
project(libsm VERSION 8.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(LIBSM_TESTS "Compile and make tests for LIBSM?" ON)

add_subdirectory(src)

add_subdirectory(examples)
if (LIBSM_TESTS)
    set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
    add_subdirectory(lib/cpputest)
    add_subdirectory(test)
endif(LIBSM_TESTS)

# include version lib
include(${PROJECT_SOURCE_DIR}/tooling/version/GitVersion.cmake)
CheckGitSetup("LIBSM") #creates git_version cmake lib
target_link_libraries(libsm PUBLIC git_version_libsm)
