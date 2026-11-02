cmake_minimum_required(VERSION 3.10)

project(Test LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Project root
get_filename_component(ROOT_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
    ABSOLUTE
)

# Find all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${ROOT_DIR}/src/*.c"
)

# Create executable
add_executable(test
    main.c
    ${SOURCES}
)

# Header search paths
target_include_directories(test PRIVATE
    "${ROOT_DIR}/include"
)

target_compile_options(test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
