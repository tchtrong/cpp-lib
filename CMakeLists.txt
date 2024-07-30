cmake_minimum_required(VERSION 3.28.3)

project(cpplib VERSION 1.0)

enable_testing()
include(CTest)

configure_file(cpplibconfig.h.in cpplibconfig.h)

set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(error_flags
    "-Werror;-Wpedantic;-pedantic-errors;-Wall;-Wextra;-Wconversion;-Wshadow;-Wformat=2"
)
set(sanitize_flags
    "-fsanitize=address,leak,undefined;-fno-omit-frame-pointer;-g;-O0")

add_library(cpplib_compiler_flags INTERFACE)
target_compile_features(cpplib_compiler_flags INTERFACE cxx_std_20)
target_compile_options(
  cpplib_compiler_flags
  INTERFACE "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${error_flags}>>"
            "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4;-WX>>")

add_library(cpplib_sanitizer_flags INTERFACE)
target_compile_options(
  cpplib_sanitizer_flags
  INTERFACE "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${sanitize_flags}>>")
target_link_options(cpplib_sanitizer_flags INTERFACE
                    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${sanitize_flags}>>")

add_subdirectory("cpplib_headers")
add_subdirectory("tests")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC cpplib_headers)
target_link_libraries(${PROJECT_NAME} INTERFACE cpplib_compiler_flags)
target_link_libraries(${PROJECT_NAME}
                      INTERFACE "$<$<CONFIG:Debug>:cpplib_sanitizer_flags>")

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
