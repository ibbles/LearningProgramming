Building a CMake project consists of the following steps [(1)](https://youtu.be/NDfTwOvWIao&t=441):
- Provision
	- Environment setup.
- Configure
	- Parse `CMakeLists.txt` to generate build system files.
- Build / Compile
- Test
- Install / Package


# Provisioning

Not really a CMake step, since it usually doesn't use CMake directly, but important to ensure that the following CMake steps work as they should [(1)](https://youtu.be/NDfTwOvWIao?t=566).
Setting up the local environment / build machine to prepare for the rest of the steps [(1)](https://youtu.be/NDfTwOvWIao?t=445).
Downloading source code, libraries, binaries, dependencies, tools, assets, etc from other machines [(1)](https://youtu.be/NDfTwOvWIao?t=459).
Make sure you pick things that work together [(1)](https://youtu.be/NDfTwOvWIao?t=498).
Using the same package manager for everything can help.
Using a mono-repo is another way to ensure that compatible versions of things are used together [(1)](https://youtu.be/NDfTwOvWIao?t=533)


# Configure

The configure step creates a build directory [(1)](https://youtu.be/NDfTwOvWIao?t=572).
A build directory is there the build happens.
The build directory is where CMake configuration files, intermediate build output, and build artifacts are placed.
The build directory is also called the binary directory since it is where the end result, the binaries, are placed.

## CMake Command

CMake command to create a binary directory for a source directory:
```shell
cmake -S {SOURCE_DIR} -B BINARY_DIR
```

- `{SOURCE_DIR`}: The directory where the source code, especially the root `CMakeLists.txt`, is located.
- `{BINARY_DIR}`: The directory where the build should happen and build artifacts be placed.


## Configuration Options

It is at this step that we can do configuration of the build [(1)](https://youtu.be/NDfTwOvWIao?t=628).
Configuration is done by adding flags to the `cmake -S {SOURCE_DIR} -B {BINARY_DIR}` invocation [(1)](https://youtu.be/NDfTwOvWIao?t=636).
The `CMakeLists.txt` scripts can add project-specific configuration options.
These options are configured with `-D{OPTION}={VALUE}` command line arguments, where `{OPTION}` is the name of the option and `{VALUE}` is the value to assign to that option.
Such options are accessed as [[CMake - Variables|variables]] in `CMakeLists.txt`.
Example where we set the option `MY_OPTION` to `MY_VALUE`:
```shell
cmake -S {SOURCE_DIR} -B {BINARY_DIR} -DMY_OPTION=MY_VALUE
```

Options are often used to enable or disable features or parts of a project, such as testing:
```shell
cmake -S {SOURCE_DIR} -B {BINARY_DIR} -DMY_PROJECT_BUILD_TESTS=ON
```

CMake also has a bunch of built-in options.
The names of the CMake built-in options all start with `CMAKE_`.


## CMake Flags

CMake also has command line flags that are not `-DOPTION=VALUE` type flags [(1)](https://youtu.be/NDfTwOvWIao?t=654).
Run `cmake --help`  to see them.
A few examples:
- `--fresh`: Redo the configuration from scratch, i.e. do not reuse state from a previous `cmake` invocation.
	- This is kinda like deleting the entire build directory, except this this keeps build artifacts.
- `-G {GENERATOR}`
	- Select what type of build system / project to generate [(1)](https://youtu.be/NDfTwOvWIao?t=686)
	- `-G Ninja`
- `-DCMAKE_CXX_FLAGS={FLAGS}` [(1)](https://youtu.be/NDfTwOvWIao?t=954).
	- Adds C++ compiler flags globally.
	- Usually better to use more specific mechanisms.
	- `-DCMAKE_CXX_FLAGS="-fsanitizer=addres -fdiagnostics-add-output=sarif"`

## Build Type

A build type is used to control what a particular build is going to be used for, often Release or Debug [(1)](https://youtu.be/NDfTwOvWIao?t=795).
It is set by passing `-DCMAKE_BUILD_TYPE={BUILD_TYPE}` to the CMake invocation.
Not setting a build type means "use the compiler defaults" [(1)](https://youtu.be/NDfTwOvWIao?t=800).
CMake are aware of:
- `Debug`
- `Release`
- `RelWithDebInfo`
- `MinSizeRel`


## Tool-chain File

The tool-chain is the compiler, linker, and other tools that source code and make it into executable code [(1)](https://youtu.be/NDfTwOvWIao?t=870).
A tool-chain file is a `.cmake` file that describe a tool-chain [(1)](https://youtu.be/NDfTwOvWIao?t=892).
Select a tool-chain file to use with `-DCMAKE_TOOLCHAIN_FILE={PATH}`.
Can be used to add compiler and linker flags [(1)](https://youtu.be/NDfTwOvWIao?t=955).


# Building

The output of the configure step is a build directory with build system files in [(1)](https://youtu.be/NDfTwOvWIao?t=1015).
Run a build with
```shell
cmake --build {BINARY_DIR} [--target {TARGET}]
```

By default it builds the `all` target, which in reality is most targets [(1)](https://youtu.be/NDfTwOvWIao?t=1060).
Use the `EXCLUDE_FROM_ALL` target property to exclude a particular target from the `all` target.
To build a target not included in `all`, or one particular target regardless of whether it is part of `all` or not,  use the `--target {TARGET}` flag.
```shell
cmake --build {BINARY_DIR} --target my_target
```

There are a few built-in targets [(1)](https://youtu.be/NDfTwOvWIao?t=1121):
- `all`
	- Build the default set of targets.
	- If you don't pass `--target` then this is what's going to be built.
- `clean`
	- Delete build artifacts so the next build will be from a clean slate.
- `codegen` [(1)](https://youtu.be/NDfTwOvWIao?t=1136)
	- Run code generation steps.
	- Used with `add_custom_command`.
- `test`
- `install`

Other flags:
- `--verbose` [(1)](https://youtu.be/NDfTwOvWIao?t=1136)
	- Print commands as they are executed.
	- Can be made the default a configure time by passing `-DCMAKE_VERBOSE_MAKEFILE=ON`.
	- This is not a Make-specific flag, it makes other build systems verbose as well.



# Testing

To test for runtime bugs [(1)](https://youtu.be/NDfTwOvWIao?t=1227).
There are two ways to run tests:
- `ctest --test-dir {BINARY_DIR}`
- `cmake --build {BINARY_DIR} --target test`

The `ctest` way have more features than the `cmake` way.

The tests must have been built before they can be run [(1)](https://youtu.be/NDfTwOvWIao?t=1312).
CMake will not by itself detect that a build must be run before trying to run a test that hasn't been built yet.


# Shipping

CMake provides the `install` target for preparing shippable packages [(1)](https://youtu.be/NDfTwOvWIao?t=1347).
CMake does some work to make sure the binaries are suitable for distribution and not just for running on your machine.

There is also a different way to make an install:
```shell
env DESTDIR={FIRST_PATH_SEGMENT} \
	cmake --install {BINARY_DIR} --prefix {SECOND_PATH_SEGMENT}
```

The above command will install the package to `{FIRST_PATH_SEGMENT}/{SECOND_PATH_SEGMENT}`.

(
something about `DESTDIR`  vs `--prefix` vs `CMAKE_PREFIX_PATH` that I don't understand [(1)](https://youtu.be/NDfTwOvWIao?t=1373)
)



# References

- [_CMake Doesn't Have to Be Painful | Simple Strategies That Work _ by Bret Brown, CppCon @ youtube.com 2025](https://youtu.be/NDfTwOvWIao) 
