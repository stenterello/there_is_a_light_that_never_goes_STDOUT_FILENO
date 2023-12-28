cmake_minimum_required(VERSION 3.10)

project(light C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(utils)

add_executable(light main.c)

if(APPLE)
    find_library(NCURSES_LIBRARY NAMES ncurses)
    target_include_directories(light PRIVATE include)
    target_link_libraries(light PRIVATE ${NCURSES_LIBRARY} m utils_library)
    target_compile_options(light PRIVATE -Wall -Werror -Wextra -D_XOPEN_SOURCE_EXTENDED)
else()
    find_library(NCURSES_LIBRARY NAMES ncursesw)
    find_library(TINFO_LIBRARY NAMES tinfo)
    target_include_directories(light PRIVATE include)
    target_link_libraries(light PRIVATE ${NCURSES_LIBRARY} ${TINFO_LIBRARY} m utils_library)
    target_compile_options(light PRIVATE -Wall -Werror -Wextra)
endif()

add_custom_target(run COMMAND ${CMAKE_CURRENT_BINARY_DIR}/light)

file(COPY text DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
