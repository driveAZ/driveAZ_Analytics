cmake_minimum_required(VERSION 3.10) # CMAKE_MATCH_<n>
# function CheckGitSetup will add lib git_version_${NAME but lower}, call it and then
# target_link_libraries(... git_version_$name)
# the git_version lib will exponse as extern char*
# * VERSION_${NAME}
# * VERSION_${NAME}_MAJOR
# * VERSION_${NAME}_MINOR
# * VERSION_${NAME}_PATCH
# * VERSION_${NAME}_GIT

find_package(Git)

# based on https://jonathanhamberg.com/post/cmake-embedding-git-hash/

set(CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
if (NOT DEFINED pre_configure_dir)
    set(pre_configure_dir ${CMAKE_CURRENT_LIST_DIR})
endif ()

if (NOT DEFINED post_configure_dir)
    set(post_configure_dir ${CMAKE_BINARY_DIR}/generated)
endif ()

set(pre_configure_file ${pre_configure_dir}/git-version.c.in)
set(pre_configure_file_h ${pre_configure_dir}/git-version.h.in)

function(CheckGitWrite ver)
    file(WRITE ${CMAKE_BINARY_DIR}/git-state.txt ${ver})
endfunction()

function(CheckGitRead ver)
    if (EXISTS ${CMAKE_BINARY_DIR}/git-state.txt)
        file(STRINGS ${CMAKE_BINARY_DIR}/git-state.txt CONTENT)
        LIST(GET CONTENT 0 var)

        set(${ver} ${var} PARENT_SCOPE)
    endif ()
endfunction()


# from https://www.foonathan.net/2016/03/cmake-install/

function(nogitVersion)
    if(CMAKE_PROJECT_VERSION MATCHES "^([0-9]+)\.([0-9]+)\.([0-9]+)$")
        set(GIT_VERSION_MAJOR ${CMAKE_MATCH_1} CACHE STRING "major version of libsm" FORCE)
        set(GIT_VERSION_MINOR ${CMAKE_MATCH_2} CACHE STRING "minor version of libsm" FORCE)
        set(GIT_VERSION_PATCH ${CMAKE_MATCH_3} CACHE STRING "patch version of libsm" FORCE)
        set(GIT_VERSION_GIT -nogit-unknown CACHE STRING "extra version of libsm" FORCE)
    else()
        message( WARNING "can't get version from git, and '${CMAKE_PROJECT_VERSION}' nor parsable")
    endif()
endfunction()



function(CheckGitVersion)
    set(LIB ${arg1})
    if(GIT_EXECUTABLE)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --match "v[0-9]*.[0-9]*.[0-9]*" --dirty
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE DESCRIBE
            RESULT_VARIABLE ERROR_CODE
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(DESCRIBE MATCHES "^v([0-9]+)\.([0-9]+)\.([0-9]+)$")
            set(GIT_VERSION_MAJOR ${CMAKE_MATCH_1} CACHE STRING "major version of libsm" FORCE)
            set(GIT_VERSION_MINOR ${CMAKE_MATCH_2} CACHE STRING "minor version of libsm" FORCE)
            set(GIT_VERSION_PATCH ${CMAKE_MATCH_3} CACHE STRING "patch version of libsm" FORCE)
            set(GIT_VERSION_GIT "" CACHE STRING "extra version of libsm" FORCE)
        elseif(DESCRIBE MATCHES "^v([0-9]+)\.([0-9]+)\.([0-9]+)(-[0-9]+-g.*)$")
            # matches
            # 1: major
            # 2: minor
            # 3: patch
            # 4: commits since tag, -g$hash, and -dirty
            set(GIT_VERSION_MAJOR ${CMAKE_MATCH_1} CACHE STRING "major version of libsm" FORCE)
            set(GIT_VERSION_MINOR ${CMAKE_MATCH_2} CACHE STRING "minor version of libsm" FORCE)
            set(GIT_VERSION_PATCH ${CMAKE_MATCH_3} CACHE STRING "patch version of libsm" FORCE)
            execute_process(
                COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE BRANCH
                RESULT_VARIABLE ERROR_CODE
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            set(GIT_VERSION_GIT "${CMAKE_MATCH_4}-${BRANCH}" CACHE STRING "extra version of libsm" FORCE)
        else()
            nogitVersion()
        endif()
    else()
        nogitVersion()
    endif()


    set(GIT_VERSION "v${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}${GIT_VERSION_GIT}"
        CACHE STRING "version of b2v libsm" FORCE)

    if(GIT_VERSION_GIT STREQUAL "-nogit-unknown")
        message(WARNING "Failed to determine version from Git tags. Using cmake project version \"${GIT_VERSION}\".")
    endif()


    CheckGitRead(GIT_VER_CACHE)

    if ("${GIT_VERSION}" STREQUAL "${GIT_VER_CACHE}" AND EXISTS ${post_configure_file})
        message(STATUS " using cached version: ${GIT_VERSION}")
    else()
        message(STATUS " GIT_VERSION:     ${GIT_VERSION}")

        configure_file(${pre_configure_file} ${post_configure_file} @ONLY)
        configure_file(${pre_configure_file_h} ${post_configure_file_h} @ONLY)

        CheckGitWrite(${GIT_VERSION})
    endif()

endfunction()
function(CheckGitSetup GIT_VERSION_NAME)
    string(TOLOWER ${GIT_VERSION_NAME} git_version_name)
    set(post_configure_file_h ${post_configure_dir}/${git_version_name}-version.h)
    set(post_configure_file ${post_configure_dir}/${git_version_name}-version.c)

    add_custom_target(AlwaysCheckGit-${GIT_VERSION_NAME} COMMAND ${CMAKE_COMMAND}
        -DRUN_CHECK_GIT_VERSION=1
        -Dpre_configure_dir=${pre_configure_dir}
        -Dpost_configure_file_h=${post_configure_file_h}
        -Dpost_configure_file=${post_configure_file}
        -DGIT_VERSION_NAME=${GIT_VERSION_NAME}
        #-DGIT_HASH_CACHE=${GIT_HASH_CACHE}
        -P ${CURRENT_LIST_DIR}/GitVersion.cmake
        BYPRODUCTS ${post_configure_file}
        )

    add_library(git_version_${git_version_name} ${post_configure_file} ${post_configure_file_h})
    target_include_directories(git_version_${git_version_name} PUBLIC ${post_configure_dir})
    add_dependencies(git_version_${git_version_name} AlwaysCheckGit-${GIT_VERSION_NAME})

    CheckGitVersion()
endfunction()

# This is used to run this function from an external cmake process.
if (RUN_CHECK_GIT_VERSION)
    CheckGitVersion()
endif ()
