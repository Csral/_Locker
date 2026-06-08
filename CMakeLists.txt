cmake_minimum_required(VERSION 3.20)
project(_Locker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(locker
    src/main.cpp
    src/locker.cpp
    src/shuffle.cpp
    src/reshuffle.cpp
)

target_include_directories(locker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})