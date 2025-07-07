# Configuration for color messages in terminal from CMake
#
# Include in a CMake file:
# include(colours.cmake)
#
# Usage:
# message("${ttyCg}This is a message in bright green${ttyN}")
#
# Reference:
# https://stackoverflow.com/a/79249843

if( ( NOT WIN32 ) AND ( NOT DEFINED ENV{NO_COLOR} ) )
    if( ( DEFINED ENV{CLICOLOR_FORCE} ) OR ( DEFINED ENV{COLORTERM} ) OR ( DEFINED ENV{COLORFGBG} ) OR ( DEFINED ENV{CLICOLOR} ) )
        execute_process ( COMMAND bash -c "tput sgr0   " OUTPUT_VARIABLE ttyN  )   # normal (remove all styles and colors)
        execute_process ( COMMAND bash -c "tput smul   " OUTPUT_VARIABLE ttySu )   # style:underline
        execute_process ( COMMAND bash -c "tput bold   " OUTPUT_VARIABLE ttySb )   # style:bold
        execute_process ( COMMAND bash -c "tput rev    " OUTPUT_VARIABLE ttySi )   # style:inverted
        execute_process ( COMMAND bash -c "tput setaf 1" OUTPUT_VARIABLE ttyCr )   # color:red
        execute_process ( COMMAND bash -c "tput setaf 2" OUTPUT_VARIABLE ttyCg )   # color:green
        execute_process ( COMMAND bash -c "tput setaf 3" OUTPUT_VARIABLE ttyCy )   # color:yellow
        execute_process ( COMMAND bash -c "tput setaf 4" OUTPUT_VARIABLE ttyCb )   # color:blue
        execute_process ( COMMAND bash -c "tput setaf 5" OUTPUT_VARIABLE ttyCm )   # color:magenta
        execute_process ( COMMAND bash -c "tput setaf 6" OUTPUT_VARIABLE ttyCc )   # color:cyan
    endif()
endif()

