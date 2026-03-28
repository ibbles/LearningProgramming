Tests are in a separate directory from your production code [(1)](https://youtu.be/NDfTwOvWIao?t=3039).
It is common to use a testing framework, such as Google Test [(1)](https://youtu.be/NDfTwOvWIao?t=3055).
Google Test provides the `GTest::gtest` library target that our tests should link against.
Chose a test name that is very specific, and often verbose, to make it clear what is being tested [(1)](https://youtu.be/NDfTwOvWIao?t=3093).
Example: `my_library.my_module.tests.some_feature`.
Tests are consumers of the library being tested, so it should link with it using the public, `::`, `ALIAS` name [(1)](https://youtu.be/NDfTwOvWIao?t=3093).
A test must be marked as a test to CMake, so that `ctest` can know that it should be run as part of testing [(1)](https://youtu.be/NDfTwOvWIao?t=3159).

`{PROJECT_ROOT}/tests/CMakeLists.txt`:
```cmake
include(GoogleTest)
find_package(GTest REQUIRED)
add_executable(my_project_my_test)
target_sources(my_project_my_test
	PRIVATE
		my_test.cpp)
target_link_libraries(
	my_project_my_test
	PRIVATE
		my_project::my_library GTest::gtest GTest::gtest_main
)
gtest_discover_tests(my_test.cpp)
```

# References

- [_CMake Doesn't Have to Be Painful | Simple Strategies That Work _ by Bret Brown, CppCon @ youtube.com 2025](https://youtu.be/NDfTwOvWIao) 
