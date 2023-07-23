cmake_minimum_required(VERSION 3.22)
project(42sh VERSION 1.0.0 LANGUAGES C)

add_library(common_options INTERFACE)
target_compile_options(common_options INTERFACE
    "-Wall"
    "-Wextra"
    "-Wvla"
    "-pedantic"

    #Release CFLAGS
    $<$<CONFIG:Release>:-Werror>
    $<$<CONFIG:Release>:-O3>
    

    #Debug CFLAGS
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-fno-inline>
    $<$<CONFIG:Debug>:-fsanitize=address>
	$<$<CONFIG:Debug>:-O0>
)

target_link_libraries(common_options INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address>
)
add_compile_definitions(common_options INTERFACE
    _POSIX_C_SOURCE=200809L
)

add_executable(42sh src/main.c)
set_target_properties(42sh PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(42sh PRIVATE
    common_options
    42sh2
)

add_subdirectory(src)
