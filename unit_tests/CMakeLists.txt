cmake_minimum_required(VERSION 3.10)

project(unitTests LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c"
)

add_executable(utils_test utils_test.c ${SOURCES})

foreach(target utils_test)

  target_include_directories(${target} PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include"
  )

  target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
    )

  target_link_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )

endforeach()
