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

set(EXEC_NAME "generate_signal")
add_executable(${EXEC_NAME}
    ${EXEC_NAME}.c
    ${SOURCES}
)
target_include_directories(${EXEC_NAME} PRIVATE
    "${ROOT_DIR}/include"
)
target_link_libraries(${EXEC_NAME} PRIVATE m)

set(EXEC_NAME "resample")
add_executable(${EXEC_NAME}
    ${EXEC_NAME}.c
    ${SOURCES}
)
target_include_directories(${EXEC_NAME} PRIVATE
    "${ROOT_DIR}/include"
)
target_link_libraries(${EXEC_NAME} PRIVATE m)

target_compile_options(generate_signal PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
