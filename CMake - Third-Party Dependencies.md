CMake can help with management of third-party dependencies of a project [(2)](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#guide:Using%20Dependencies%20Guide).
Specifically, CMake can:
- Fetch source code, or other files, that are not part of the project's repository.
	- This is done with Fetch Content [(1)](https://cmake.org/cmake/help/latest/module/FetchContent.html).
- Finding and configuring pre-built dependency libraries.
	- This is done with Find Package [(3)](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)


# Find Package

The `find_package` command is used to find and configure pre-built library packages.
This makes it possible to link CMake-built targets against them.
For `find_package` to work either a `FindPACKAGE.cmake`, `PACKAGEConfig.cmake`, or `PACKAGE-config.cmake` file must be available.

A few basic usage examples, where the library to find is named `SomeLibrary`:
```cmake
find_package(SomeLibrary)
find_package(SomeLibrary REQUIRED)
find_package(SomeLibrary COMPONENTS SomeFeature AnotherFeature)
```

In this case the `FindPACKAGE.cmake` file would be named `FindSomeLibrary.cmake`.
In this case the `PACKAGEConfig.cmake` file would be named `SomeLibraryConfig.cmake`.

In case the third-party library can have different names use the `NAMES` parameter list:
```cmake
find_package(SomeLibrary NAMES SomeLibrary SomeLibraryLinux SomeLibraryMacOS SomeLibraryWin64)
```


A complete example:
```cmake
cmake_minimum_required(CMAKE_VERSON 3.10)
project(MyProject)

# Tell CMake where we store our Find${PACKGE}.cmake files.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SomeLibrary REQUIRED)
add_executable(MyProgram main.cpp)
target_link_libraries(MyProgram PRIVATE SomeLibrary::LibName)
```


## Imported Target

The result of `find_package` is an imported target.
An imported target is a target that CMake doesn't generate build instructions for.
A target that is assumed to have been built already.
An imported target is linked against with `target_link_libraries`, just like regular CMake build targets.


## Config Mode

This mode is used when a `ConfigurePACKAGE.cmake` file is found.
It is mainly used when the third-party library is build with CMake since CMake can generate the `ConfigurePACKAGE.cmake`.
The `ConfigurePACKAGE.cmake` file is usually placed in `lib/cmake/PACKAGE/`.


## Module Mode

This mode is used when a `Find${PACKAGE}.cmake` file is found.
It is mainly used when the third-party library is not built with CMake, or when explicit control over the search and configuration process is required.
`Find${PACKAGE}.cmake` files are searched for in `CMAKE_MODULE_PATH`.



## Search Directories

`find_package` has a list of directories that it by default will search for the `FindPACKAGE.cmake` and `ConfigurePACKAGE.cmake` files.
We can control this list with arguments to the `find_package` call and through CMake cache variables passed on the `cmake` argument list or set in a CMake GUI.

The `CMAKE_PREFIX_PATH` CMake variable is a list of directories to search for packages in.
It can be set when running CMake:
```shell
cmake -DCMAKE_PREFIX_PATH=/opt/SomeLibrary/:/opt/AnotherLibrary/
```

The `CMAKE_PREFIX_PATH` environment variable works just like the CMake variable with the same name, but uses a platform-specific separator (`;` or `:`) instead of the CMake list element separator.

An both cases above, CMake expects to find `/lib/cmake/SomeLibrary/SomeLibraryConfig.cmake` within one of the search directories.

If you know where the config or find file for a particular library is then you can set the `<PACKAGE>_DIR` CMake variable to the directory that contains the `<PACKAGE>Config.cmake` or `Find<PACKAGE>.cmake` file.
For example `/opt/SomeLibrary/lib/cmake/SomeLibrary/`.

# Fetch Content

CMake provides a way to download source code, or other files, from hosting sites.
This can be done with the Fetch Content family of functions [(1)](https://cmake.org/cmake/help/latest/module/FetchContent.html).
To use Fetch Content the `CMakeLists.txt` must contain `include(FetchContent)`.
The two main functions are:
- `FetchContent_Declear`: Tell CMake which content we may want to use and how to get it.
- `FetchContent_MakeAvailable`: Tell CMake to download the content actually needed.


## `FetchContent_Declare`

Fetch Content can download content from a number of sources.
Here we will focus on Git.

`FetchContent_Decleare` is used to tell CMake where to find some content that we may need to download, and how to configure it.
`FetchContent_Decleare` needs to know three pieces of information:
- The name of the dependency.
	- This is the name that will be used later to make the declared dependency available, i.e. to download it and link against fetched targets.
	- Cannot contain spaces.
- The Git repository URL.
- The Git revision to download.
	- This can be a tag, branch, or commit hash. (I assume.)

This information is passed to `FetchContent_Declare` using named parameters:
```cmake
include(FetchContent)

# Declare SomeLibrary as a GitHub hosted project.
FetchContent_Declare(
	SomeLibrary
	GIT_REPOSITORY https://github.com/SomeMaintainer/SomeLibrary.git
	GIT_TAG 1.0.0  # Tag names are easy, but commit hashes are safer.
)

# Declare AnotherLibrary as a web hosted Zip archive.
FetchContent_Declare(
	AnotherLibrary
	URL https://www.anotherlibrary.com/releases/v1.0.0.zip
	URL_HASH MD5=0123456789abcdef
)
```

Prefer to use a commit hash rather than a tag name for `GIT_TAG`.
A commit hash is more difficult for an adversary to replace the contents of.

## `FetchContent_MakeAvailable`

`FetchContent_MakeAvaiable` is used as follows, which continues the example in the preceding section [1](https://cmake.org/cmake/help/latest/module/FetchContent.html#command:fetchcontent_makeavailable):
```cmake
FetchContent_MakeAvailable(SomeLibrary AnotherLibrary)
```

Any build targets found in the content is added to the project.
This is done by calling `add_subdirectory` and `find_package` behind the scenes.
It is up to the third-partly library do decide what happens inside its `CMakeLists.txt`.
It will typically declare one or more libraries with `add_library` that we can link our targets against.

Perhaps like this:
```cmake
FetchContent_MakeAvailable(SomeLibrary AnotherLibrary)
target_link_libraries(MyTarget SomeLibrary::SomeLibrary AnotherLibrary::AnotherLibrary)
```


If the third-party library provides configuration options through cache variables then those should be set before calling `FetchContent_MakeAvailable`.
```cmake
set(SOMELIMBRARY_BUILD_TESTS OFF)  # Regular variable OK, or must it be a CACHE variable?
FetchContent_MakeAvailable(SomeLibrary)
```

By default `FetchContent_MakeAvailable` will create three subdirectories in `_deps`:
- `SomeLibrary-src`
	- Where the files pointed-to by the `FetchContent_Declare` call.
	- This can be a Git working copy.
- `SomeLibrary-build`
	- Build directory for the library.
	- Just like any other CMake build directory.
	- It has `SomeLibrary-src` as its source directory.
- `SomeLibrary-subbuild`
	- Not sure. Something used during the `FetchContent_MakeAvailable` process I think.

## No `add_subdirectory`

In some cases we do not want to call `add_subdirectory` on the newly downloaded third-party source directory.
We can accomplish this by telling `FetchContent_MakeAvailable` to look for the `CMakeLists.txt`, i.e. what directory to pass to `add_subdirectory`, in a directory that doesn't exist.
Then CMake will determine that it can't call `add_subdirectory` since `add_subdirectory` requires a `CMakeLists.txt`.
The way we pass that information to `FetchContent_MakeAvailable` is through a named parameter passed to `FetchContent_Declare`:

```cmake
# Declare SomeLibrary as project that add_subdirectory should not be called for
FetchContent_Declare(
	SomeLibrary
	GIT_REPOSITORY https://github.com/SomeMaintainer/SomeLibrary.git
	GIT_TAG 1.0.0  # Tag names are easy, but commit hashes are safer.
	SOURCE_SUBDIR "some-directory-that-does-not-exist"
)

FetchContent_MakeAvailable(SomeLibrary)
```

Even though `FetchContent_MakeAvailable` doesn't run `add_subdirectory`, configuration variables for that library is still being set.
Such as `SomeLibrary_SOURCE_DIR` and `SomeLibrary_BINARY_DIR`.
This can be used, for example, to create a header-only library.
```cmake
# Specify a SOURCE_SUBDIR that doesn't exist to tell FetchContent_MakeAvailable
# that it should not call add_subdirectory.
FetchContent_Declare(
	SomeLibrary
	GIT_REPOSITORY https://github.com/SomeMaintainer/SomeLibrary.git
	GIT_TAG 1.0.0  # Tag names are easy, but commit hashes are safer.
	SOURCE_SUBDIR "supress-add_subdirectory" # Assume this directory doesn't exist.
)

# Will not call add_subdirectory since there is no directory named
# supress-add_subdirectory with a CMakeLists.txt in it.
FetchContent_MakeAvailable(SomeLibrary)

# The third-party library sources are now available, but no target to link against
# has been created yet. Create an INTERFACE library with the source directory as
# one of its include directories.
add_library(SomeLibrary INTERFACE)
target_include_directories(SomeLibrary SYSTEM INTERFACE "${SomeLibrary_SOURCE_DIR}")
export(TARGETS SomeLibrary FILE SomeLibrary.cmake)
add_library(SomeLibrary::HeaderOnly ALIAS SomeLibrary)
```


# External Project

Used to download content at build time [(5)](https://cmake.org/cmake/help/latest/module/ExternalProject.html)

# References

- 1: [_FetchContent_ @ cmake.org](https://cmake.org/cmake/help/latest/module/FetchContent.html)
- 2: [_Using Dependencies Guide_ @ cmake.org](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#guide:Using%20Dependencies%20Guide)
- 3: [_find_package_ @ cmake.org](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package)
- 4: [_cmake-packages_ @ cmake.org](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html)
- 5: [_External Project_ @ cmake.org](https://cmake.org/cmake/help/latest/module/ExternalProject.html)
