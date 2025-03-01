cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(WavReader VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(read_wav src/main.cpp src/WavReader.cpp)

if (MSVC)
  target_compile_options(read_wav PRIVATE /W4 /WX)
else()
  target_compile_options(read_wav PRIVATE -Wall -Wextra -Werror)
endif()

