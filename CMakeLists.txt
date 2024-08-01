# CMake 3.30 is required for C++23 `import std` support; we use 3.29.20240416
# here so that in-development versions satisfy it.
cmake_minimum_required(VERSION 3.30.1 FATAL_ERROR)

# Set experimental flag to enable `import std` support from CMake. This must be
# enabled before C++ language support.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    # This specific value changes as experimental support evolves. See
    # `Help/dev/experimental.rst` in the CMake source corresponding to
    # your CMake build for the exact value to use.
    "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_CXX_EXTENSIONS 0)

# C++ needs to be enabled.
project(import_std LANGUAGES CXX)
enable_testing()
include(CTest)
configure_file(cpplibconfig.hpp cpplibconfig.h)
add_subdirectory("tests")

set(error_flags
    "-Werror;-Wpedantic;-pedantic-errors;-Wall;-Wextra;-Wconversion;-Wshadow;-Wformat=2"
)
set(sanitize_flags
    "-fsanitize=address,leak,undefined;-fno-omit-frame-pointer;-O0")

add_library(cpplib_compiler_flags INTERFACE)
target_compile_features(cpplib_compiler_flags INTERFACE cxx_std_26)
target_compile_options(
  cpplib_compiler_flags
  INTERFACE
    "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>;$<BUILD_INTERFACE:${error_flags}>"
)
target_link_options(cpplib_compiler_flags INTERFACE
                    "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++;-lc++abi>")

add_library(cpplib_sanitizer_flags INTERFACE)
target_compile_options(
  cpplib_sanitizer_flags
  INTERFACE "$<BUILD_INTERFACE:$<$<CONFIG:Debug>:${sanitize_flags}>>")
target_link_options(cpplib_sanitizer_flags INTERFACE
                    "$<BUILD_INTERFACE:$<$<CONFIG:Debug>:${sanitize_flags}>>")

# Tell CMake that we explicitly want `import std`. This will initialize the
# property on all targets declared after this to 1
set(CMAKE_CXX_MODULE_STD 1)

# Make a library.
add_library(cpplib STATIC)
# Add sources.
target_sources(
  cpplib
  PUBLIC FILE_SET
         cpplib_modules
         TYPE
         CXX_MODULES
         FILES
         array.cppm
         compare.cppm
         concepts.cppm
         doubly_list.cppm
         iterator.cppm
         cpplib.cppm)
target_link_libraries(cpplib PUBLIC cpplib_compiler_flags
                                    cpplib_sanitizer_flags)
