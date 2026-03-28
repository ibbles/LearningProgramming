# Minimal


The first line should set the CMake compatibility version / feature flag [(1)](https://youtu.be/NDfTwOvWIao?t=2080).
This tells CMake to be bug- and feature compatible with a particular CMake version.
It is good to keep this bumped to see deprecation notices, new features enabled, performance improvements [(1)](https://youtu.be/NDfTwOvWIao?t=2112).
If you don't know what to set it to, run `cmake --version` on your development machine and set that [(1)](https://youtu.be/NDfTwOvWIao?t=2132).
```cmake
cmake_minimum_required(VERSION {VERSION})
```


The second line should be the project statement [(1)]([https://youtu.be/NDfTwOvWIao?t=2168](https://youtu.be/NDfTwOvWIao?t=2163)).
This is what loads the file pointed to by `CMAKE_TOOLCHAIN_FILE` [(1)](https://youtu.be/NDfTwOvWIao?t=2179).
This is what initializes a bunch of system CMake variables such as OS information.
```cmake
project({PROJECT_NAME} [DESCRIPTION {PROJECT_DESCRIPTION}] LANGUAGES CXX [VERSION {PROJECT_VERSION}])
```


Enable the test target in your top-level `CMakeLists.txt` [(1)](https://youtu.be/NDfTwOvWIao?t=2215).
```cmake
include(CTest)
# or
enable_testing()
```

So the final minimal CMake project is:
```cmake
cmake_minimum_required(VERSION {VERSION})
project({PROJECT_NAME} [DESCRIPTION {PROJECT_DESCRIPTION}] LANGUAGES CXX [VERSION {PROJECT_VERSION}])
include(CTest)
```

Example:
```cmake
cmake_minimum_required(VERSION 3.25)
project(my_project DESCRIPTION "My project." LANGUAGES CXX VERSION 1.2.3)
include(CTest)
```

# Multiple Directories

A CMake project can be spread out over multiple subdirectories [(1)](https://youtu.be/NDfTwOvWIao?t=2242).
The `add_subdirectory` transfer control from the current `CMakeLists.txt` to another `CMakeLists.txt` in a subdirectory.
```cmake
add_subdirectory({DIRECTORY_NAME})
```

Can be controlled by an option.
```cmake
option(MY_PROJECT_BUILD_EXAMPLES "Set to ON to build examples." OFF)
if (MY_PROJECT_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
```



# References

- [_CMake Doesn't Have to Be Painful | Simple Strategies That Work _ by Bret Brown, CppCon @ youtube.com 2025](https://youtu.be/NDfTwOvWIao) 
