# create the testing file and list of tests
set(tests doubly_list.cpp iterator.cpp merge.cpp erase_clear.cpp)
create_test_sourcelist(doubly_list_tests doubly_list_tests.cpp ${tests})

# add the executable
add_executable(doubly_list_tests_exe ${doubly_list_tests})
target_link_libraries(
  doubly_list_tests_exe PUBLIC cpplib_headers cpplib_compiler_flags
                               cpplib_sanitizer_flags)

# Add all the ADD_TEST for each test
foreach(test ${tests})
  cmake_path(GET test STEM LAST_ONLY TName)
  add_test(NAME "doubly_list-${TName}" COMMAND doubly_list_tests_exe ${TName})
endforeach()
