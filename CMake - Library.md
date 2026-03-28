# Declare A Library

A library target is declared with `add_library`, passing in the name of the library [(1)](https://youtu.be/NDfTwOvWIao?t=2283).
```cmake
add_library({LIBRARY_NAME})
```

Example:
```cmake
add_library(my_project_my_library)
```

If the library is mean to be used by other projects then we should add an alias, which is also done with `add_library`.
```cmake
add_library({PROJECT_NAME}::{LIBRARY_NAME} ALIAS {LIBRARY_NAME})
```

Example:
```cmake
add_library(my_project::utils ALIAS my_project_utils)
```

The non-alias name, the named passed to the first `add_library`, is the internal / private name.
The alias name is the external / public name of the library, the name used by consumers of the library [(1)](https://youtu.be/NDfTwOvWIao?t=2295).
The  alias name should always contain a double-colon, `::`.
Use the internal name when configuring the library.
Use the external name when consuming the library.
Reasons [(1)](https://youtu.be/NDfTwOvWIao?t=2343).
- `target_link_libraries` will detect the `::` and do extra checking, e.g. making sure the target exists.
- If the library name doesn't have `::` then CMake will assume that the library exists _somewhere_ and will add it to the link line and hope for the best.


# Add Source Files

`target_sources` is used to associate source code to be compiled with a target [(1)](https://youtu.be/NDfTwOvWIao?t=2402).
```cmake
target_sources({TARGET_NAME} PRIVATE {LIST OF SOURCE FILES})
```

Example:
```cmake
target_sources(my_project_my_library PRIVATE my_source.cpp)
```
 


We can also associate header files with a target [(1)](https://youtu.be/NDfTwOvWIao?t=2446).
This is a public thing, consumers of the library need to know about the header files, so the visibility should be `PUBLIC`.
We tell CMake that we are about to list header files with the `FILE_SET HEADERS` arguments.
We tell CMake what the base directory is, i.e. where the root of the files are, and then list the files themselves.
The files should be somewhere in the directory tree rooted at the `BASE_DIRs`.
When the library is installed then only the path after the `BASE_DIRS` will be used.

```cmake
target_sources(
	{TARGET_NAME}
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/{DIR_PATH})
		FILES ${CMAKE_CURRENT_SOURCE_DIR/{DIR_PATH}/{FILE_PATH}}
)
```

Example:
```cmake
target_sources(
	my_library
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/utils.hpp
)
```

In this example the thing being installed is `utils/utils.hpp` since that is the part of the path this is not the `BASE_DIRS`.


`target_sources({LIBRARY_NAME} PUBLIC FILE_SET HEADERS ...)` is an alternative to `target_include_directories` [(1)](https://youtu.be/NDfTwOvWIao?t=2537).
An alternative that let CMake help you detect cases where `target_include_directories` would find the wrong headers.

CMake has a feature to to verify that header files can build on their own [(1)](https://youtu.be/NDfTwOvWIao?t=2651).
```cmake
set_target_properties(
	{TARGET_NAME}
	PROPERTIES
		VERIFY_INTERFACE_HEADER_SETS ON
)
```

 Creates a build target named `all_verify_interface_header_sets` that is not part of the `all` target set.
 Build it with
 ```shell
 cmake -B BINARY_DIR --target all_verify_interface_header_sets
 ```


# Install

Install is complicated, so let's keep it simple for now [(1)](https://youtu.be/NDfTwOvWIao?t=2774).
```cmake
include(GNUInstallDirs)
install(
	TARGETS {TARGET_NAME}
	EXPORT {TARGET_NAME}
	FILE_SET HEADERS
)
install(
	EXPORT {TARGET_NAME}
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
	NAMESPACE "{MY_PROJECT}::"
	FILE "my_project-config.cmake"
	EXPORT_PACKAGE_DEPENDENCIES
)
set_target_properties(
	{TARGET_NAME}
	PROPERTIES
		EXPORT_NAME "{MY_PROJECT}"
)
```

# References

- [_CMake Doesn't Have to Be Painful | Simple Strategies That Work _ by Bret Brown, CppCon @ youtube.com 2025](https://youtu.be/NDfTwOvWIao) 
