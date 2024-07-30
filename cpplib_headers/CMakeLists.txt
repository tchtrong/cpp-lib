add_library(cpplib_headers INTERFACE)

target_include_directories(
    cpplib_headers
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    cpplib_headers
    INTERFACE
    cpplib_compiler_flags
    "$<$<CONFIG:Debug>:cpplib_sanitizer_flags>"
)
