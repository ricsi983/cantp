if(EXISTS "/Users/hanaszrichard/projects/cantp/build/hello_test[1]_tests.cmake")
  include("/Users/hanaszrichard/projects/cantp/build/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()
