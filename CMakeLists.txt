cmake_minimum_required(VERSION 3.20)
project(_Locker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Target
add_executable(locker
    src/main.cpp
    src/locker.cpp
    src/shuffle.cpp
    src/reshuffle.cpp
)

target_include_directories(locker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Build type flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    message(STATUS "Debug build -- enabling sanitizers and debug defines")

    if(MSVC)
        target_compile_options(locker PRIVATE /W4 /WX)
    else()
        target_compile_options(locker PRIVATE -g -Og -Wall -Wextra -Wpedantic -Werror)
        if(NOT MINGW)
            target_compile_options(locker PRIVATE -fsanitize=address,undefined)
            target_link_options(locker PRIVATE -fsanitize=address,undefined)
        endif()
    endif()

    target_compile_definitions(locker PRIVATE
        LOCKER_ARG_PARSER_DEBUG
        LOCKER_APP_CONFIG_PRINTER_REQUIRED
    )

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")

    message(STATUS "Release build -- optimizations on, symbols stripped")

    if(MSVC)
        target_compile_options(locker PRIVATE /O2 /W0 /GL)
        target_link_options(locker PRIVATE /LTCG)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else()
        target_compile_options(locker PRIVATE -O2 -flto -w)
        target_link_options(locker PRIVATE -flto -s)
        if(MINGW)
            target_link_options(locker PRIVATE -static -static-libgcc -static-libstdc++)
        endif()
    endif()
endif()

# Packaging
set(CPACK_PACKAGE_NAME "Locker")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_GENERATOR "ZIP")

install(TARGETS locker RUNTIME DESTINATION bin)
include(CPack)