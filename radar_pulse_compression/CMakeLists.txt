cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(chirp chirp.c)
add_executable(fft fft.c)
add_executable(received received.c)

foreach(target chirp fft received)

  target_link_libraries(${target} PRIVATE m)

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
