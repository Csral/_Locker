# _Locker
Core idea: Given an unique and random number, a bit count, a reshuffle count, a reshuffle direction - lockout any file.

This tool **does not** guarantee cryptographic security. It is intended for controlled file transformation and experimentation.

## Warning
Always keep backups of original files and choose to remove the original file only after integrity check is performed.
The integrity check feature is not currently implemented.

### Manual integrity verification (recommended)
To verify correctness manually:

1. Encode the file.
2. Decode the output file using the exact same parameters.
3. Compare the decoded file with the original one, ideally using tools such as `diff`.

If the files are different then the integrity verification has failed, i.e., locker cannot be used either on the given file or with current configuration or both.

**NOTE:** If `diff` command produces any output, then the files are different.

### Important
**You are required to store all parameters used during encoding as they are required for decoding.**

# Getting started
Use -h or --help to get started.

# Releases
**NOTE:** Prebuilt binaries are not currently provided. Build from source is required.

# Building from source

## Prerequisites
* CMAKE 3.20 or above.
* A C++ compiler supporting C++17 standard or above.

## Building
Clone the source tree from `https://github.com/Csral/_Locker.git`.
Open the directory in a terminal and execute the following commands:
* `cmake -B build -DCMAKE_BUILD_TYPE=Release`
* `cmake --build build`

### Debug mode building
To build the binary in debug mode, execute the following commands instead:
* `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
* `cmake --build build`

**NOTE: CMake will specify where the executable output is stored. It is typically located in the build directory or one of its subdirectories.**

# License
This project is licensed under GPLv3. For more information, see the [LICENSE](LICENSE) file.