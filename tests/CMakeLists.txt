# create the testing file and list of tests
set(tests doubly_list.cpp)
create_test_sourcelist(cpplib_tests cpplib_tests.cxx ${tests})

# add the executable
add_executable(cpplib_tests_exe ${cpplib_tests})
target_link_libraries(
  cpplib_tests_exe PUBLIC cpplib_headers cpplib_compiler_flags
                          cpplib_sanitizer_flags)

# Add all the ADD_TEST for each test
foreach(test ${tests})
  cmake_path(GET test STEM LAST_ONLY TName)
  add_test(NAME "${TName}" COMMAND cpplib_tests_exe ${TName})
endforeach()
