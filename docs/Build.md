# Building from source

## Prerequisites
* CMAKE 3.20 or above.
* A C++ compiler supporting C++20 standard or above.
* Ninja Generator.

**NOTE:** You can use any generator to compile the source tree. However, the build script was tested with `Ninja 1.12.1` and `cmake version 4.3.3`. Handle any issues by different generators accordingly.

## Building
Clone the source tree from `https://github.com/Csral/_Locker.git`.

Open the directory in a terminal and execute the following commands:
* `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
* `cmake --build build`

### Debug mode building
To build the binary in debug mode, execute the following commands instead:
* `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug`
* `cmake --build build`

The binary should be available in build directory usually at build/locker or one of the subdirectories in build directory.