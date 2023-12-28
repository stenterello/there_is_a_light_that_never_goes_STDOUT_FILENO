cmake_minimum_required(VERSION 3.10)

project(utils_library C)

set(CMAKE_C_STANDARD 11)

set(SRCS
    writing.c
    scenes.c
    utils.c
    steptwo.c
    stepthree.c
    ft_split.c
)

add_library(utils_library ${SRCS})

if(APPLE)
    find_library(NCURSES_LIBRARY NAMES ncurses)
    target_include_directories(utils_library PRIVATE ../include)
    target_link_libraries(utils_library PRIVATE ${NCURSES_LIBRARY} m)
    target_compile_options(utils_library PRIVATE -Wall -Werror -Wextra -D_XOPEN_SOURCE_EXTENDED)
else()
    find_library(NCURSES_LIBRARY NAMES ncursesw)
    find_library(TINFO_LIBRARY NAMES tinfo)
    target_include_directories(utils_library PRIVATE ../include)
    target_link_libraries(utils_library PRIVATE ${NCURSES_LIBRARY} ${TINFO_LIBRARY} m)
    target_compile_options(utils_library PRIVATE -Wall -Werror -Wextra)
endif()

