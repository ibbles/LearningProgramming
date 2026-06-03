Wicked Engine is a C++ game engine / library.
[Web page](https://wickedengine.net/)
[GitHub page](https://github.com/turanszkij/WickedEngine)
[C++ documentation](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md)

# Build

```shell
/media/s2000/workspaces/wicked_engine/working_copy
➤ git clone git@github.com:turanszkij/WickedEngine.git .
⎇  master➤ sudo apt install libsdl2-dev

/media/s2000/workspaces/wicked_engine/build
➤ cmake ../working_copy/ -DCMAKE_BUILD_TYPE=Release
➤ cmake --build . -j
# Lots of warnings, but successfull build.

# Run a sample.
/media/s2000/workspaces/wicked_engine/build
➤ ./Samples/Example_ImGui/Example_ImGui
Created GraphicsDevice_Vulkan (424 ms)
Adapter: NVIDIA GeForce RTX 4070 Ti
[Error] wi::shadercompiler: could not load library ./libdxcompiler.so
[Error] ./libdxcompiler.so: cannot open shared object file: No such file or directory
[Error] shader compile FAILED: /media/s2000/workspaces/wicked_engine/build/shaders/spirv/ImGuiVS.cso

[Error] shader compile FAILED: /media/s2000/workspaces/wicked_engine/build/shaders/spirv/hairparticleVS.cso
[Warning] File not found: /media/s2000/workspaces/wicked_engine/build/shaders/spirv/hairparticleVS.cso

[Error] shader compile FAILED: /media/s2000/workspaces/wicked_engine/build/shaders/spirv/gaussian_splatCS.cso
[Warning] File not found: /media/s2000/workspaces/wicked_engine/build/shaders/spirv/gaussian_splatCS.cso

# And so on for many shader files.

[Warning] File not found: ../../Content/models/teapot.wiscene
fish: Job 1, './Samples/Example_ImGui/Example…' terminated by signal SIGSEGV (Address boundary error)
```

Not sure if the errors are actually because of file not found, or a follow-up error from `libdxcompiler.so` not being found.
Is that a system library, or part of Wicked Engine?
```shell
/media/s2000/workspaces/wicked_engine/build[139]
➤ find . -iname "libdxcompiler.so"
./WickedEngine/libdxcompiler.so
./Samples/Tests/libdxcompiler.so
./Samples/Template_Linux/libdxcompiler.so
./Samples/Example_ImGui_Docking/libdxcompiler.so
./Samples/Example_ImGui/libdxcompiler.so
./Editor/libdxcompiler.so
```

It's in many places.
Maybe I need to `cd` to the directory of the test I want to run.

Does the missing `.cso` files exist?
```shell
/media/s2000/workspaces/wicked_engine/build
➤ find . -iname "*.cso"
# No output.
```

So it could be both that `libdxcompiler.so` broke something, and that the files doesn't exist.
And they don't exist because `libdxcompiler.so` couldn't be found.

Let's try running the sample from its directory.
```shell
/media/s2000/workspaces/wicked_engine/build/Samples/Example_ImGui
➤ ./Example_ImGui
# Lots and lots of 'shader compiled' output. Good.
```

I get a window with a teapot.
ImGUI works.


# Editor

Testing the editor.
Started with
```shell
/media/s2000/workspaces/wicked_engine/build/Editor
➤ ./Editor
```

(
It is important to start the editor from the `Editor` directory.
Standing in the `build` directory and doing `./Editor/Editor` does not work.
)

Works.
I can't find any sample projects though.
All the samples seems to be C++ samples.

I did find a Content button in the top-right.
From that I can add stuff.
Like models and particle systems.
The `editorEmitter` / `Armature` thingy from `emitter_skinned.wiscene` is kinda buggy.
The transform gizmo is rotated so that the Z axis is pointing along the floor, moving it along Z moves the transform gizmo sideways but the particles up / down.
Weird.

I want to delete a bunch of stuff from the scene, but shift-click in the Entities list doesn't select a range of entities.
CTRL+A doesn't select all entities.
Click and drag doesn't box-select.
Shift-click does range select if I hold shift for both clicks.
No, sometimes it takes three clicks.
No, one must select from the top-down.
A range must extend from an upper entity to a lower entity, one cannot click the lower entity first and then the upper entity, that doesn't select a range.
It doesn't seem to group entities by import, it just dumps everything in the same long list.

OK, editor seems to work.

# Samples / Examples

Testing a few more of the samples / examples.
The `Example_ImGui_Docked` example was cool, showcasing a bunch of stuff.
The Linux sample was just a black screen.
Is that a problem?
`Samples/Test` is also cool.

I like this so far.

Can I make an application of my own?
A stand-alone one.


# Stand-Alone Application

Reading [WickedEngine-Documentation.md](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md).
Creating a very simple `CMakeLists.txt` with just a single-file C++ application that knows about the Wicked Engine header files include directory and links with the Wicked Engine static library.

```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake ../working_copy/ -DCMAKE_BUILD_TYPE=Release
➤ cmake --build . -j
# Lots of undeclared symbol errors on SDL stuff.
```

Such as `SDL_GetWindowSize`.
Add include of `SDL2.h` to `main.cpp`.
Now getting type conversion errors such as
```
WickedEngine/wiPlatform.h:107:35: error: invalid conversion from ‘wi::platform::window_type’ {aka ‘void*’} to ‘SDL_Window*’ [-fpermissive]
  107 |                 SDL_GetWindowSize(window, &window_width, &window_height);
      |                                   ^~~~~~
      |                                   |
      |                                   wi::platform::window_type {aka void*}
```

and still undeclared symbol errors such as `SDL_Vulkan_GetDrawableSize`.
Comparing with the example `main_SDL2.cpp` and `Test.h` I don't see anything obvious that it missing.

Is there a difference in the build command line?

`Tests`:
```shell
[ 94%] Building CXX object Samples/Tests/CMakeFiles/Tests.dir/Tests.cpp.o
cd /media/s2000/workspaces/wicked_engine/build/Samples/Tests && /usr/bin/c++ -DJPH_USE_AVX -DJPH_USE_F16C -DJPH_USE_FMADD -DJPH_USE_LZCNT -DJPH_USE_SSE4_1 -DJPH_USE_SSE4_2 -DJPH_USE_TZCNT -DLUA_USE_POSIX=1 -DNDEBUG -DSDL2=1 -DWICKED_CMAKE_BUILD -DWI_UNORDERED_MAP_TYPE=2 -D_GLIBCXX_USE_CXX11_ABI=1 -D_XM_AVX_INTRINSICS_ -D_XM_F16C_INTRINSICS_ -D_XM_FMA3_INTRINSICS_ -D_XM_SSE4_INTRINSICS_ -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/src -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/include -isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -isystem /usr/include/SDL2 -O3 -DNDEBUG -std=c++17 -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-sign-compare -fno-exceptions -fno-rtti -fno-stack-protector -fcf-protection=none -fno-stack-clash-protection -fno-stack-check -fno-asynchronous-unwind-tables -Wno-strict-aliasing -mavx -mpopcnt -mlzcnt -mbmi -mf16c -mfma -mfpmath=sse -pthread -Winvalid-pch -include /media/s2000/workspaces/wicked_engine/build/Samples/Tests/CMakeFiles/Tests.dir/cmake_pch.hxx -MD -MT Samples/Tests/CMakeFiles/Tests.dir/Tests.cpp.o -MF CMakeFiles/Tests.dir/Tests.cpp.o.d -o CMakeFiles/Tests.dir/Tests.cpp.o -c /media/s2000/workspaces/wicked_engine/working_copy/Samples/Tests/Tests.cpp

[ 94%] Building CXX object Samples/Tests/CMakeFiles/Tests.dir/main_SDL2.cpp.o
cd /media/s2000/workspaces/wicked_engine/build/Samples/Tests && /usr/bin/c++ -DJPH_USE_AVX -DJPH_USE_F16C -DJPH_USE_FMADD -DJPH_USE_LZCNT -DJPH_USE_SSE4_1 -DJPH_USE_SSE4_2 -DJPH_USE_TZCNT -DLUA_USE_POSIX=1 -DNDEBUG -DSDL2=1 -DWICKED_CMAKE_BUILD -DWI_UNORDERED_MAP_TYPE=2 -D_GLIBCXX_USE_CXX11_ABI=1 -D_XM_AVX_INTRINSICS_ -D_XM_F16C_INTRINSICS_ -D_XM_FMA3_INTRINSICS_ -D_XM_SSE4_INTRINSICS_ -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/src -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/include -isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -isystem /usr/include/SDL2 -O3 -DNDEBUG -std=c++17 -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-sign-compare -fno-exceptions -fno-rtti -fno-stack-protector -fcf-protection=none -fno-stack-clash-protection -fno-stack-check -fno-asynchronous-unwind-tables -Wno-strict-aliasing -mavx -mpopcnt -mlzcnt -mbmi -mf16c -mfma -mfpmath=sse -pthread -Winvalid-pch -include /media/s2000/workspaces/wicked_engine/build/Samples/Tests/CMakeFiles/Tests.dir/cmake_pch.hxx -MD -MT Samples/Tests/CMakeFiles/Tests.dir/main_SDL2.cpp.o -MF CMakeFiles/Tests.dir/main_SDL2.cpp.o.d -o CMakeFiles/Tests.dir/main_SDL2.cpp.o -c /media/s2000/workspaces/wicked_engine/working_copy/Samples/Tests/main_SDL2.cpp

[ 94%] Linking CXX executable Tests
cd /media/s2000/workspaces/wicked_engine/build/Samples/Tests && /media/s1300/bin/clion/clion-2023.2.1/bin/cmake/linux/x64/bin/cmake -E cmake_link_script CMakeFiles/Tests.dir/link.txt --verbose=1
/usr/bin/c++ -O3 -DNDEBUG -pthread -Wl,--dependency-file=CMakeFiles/Tests.dir/link.d CMakeFiles/Tests.dir/Tests.cpp.o CMakeFiles/Tests.dir/main_SDL2.cpp.o -o Tests  ../../WickedEngine/libWickedEngine.a -ldl ../../WickedEngine/libJolt.a ../../WickedEngine/LUA/libLUA.a ../../WickedEngine/Utility/libUtility.a ../../WickedEngine/Utility/FAudio/libFAudio.a /usr/lib/x86_64-linux-gnu/libSDL2.so
```

`wicked_engine_app`:
```shell
[ 50%] Building CXX object CMakeFiles/wicked_engine_app.dir/main.cpp.o
/usr/bin/c++  -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -O3 -DNDEBUG -MD -MT CMakeFiles/wicked_engine_app.dir/main.cpp.o -MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d -o CMakeFiles/wicked_engine_app.dir/main.cpp.o -c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp
```

Let's try adding the `Tests.cpp.o` flags to my compiler command line.
```shell

/usr/bin/c++ -DJPH_USE_AVX -DJPH_USE_F16C -DJPH_USE_FMADD -DJPH_USE_LZCNT -DJPH_USE_SSE4_1 -DJPH_USE_SSE4_2 -DJPH_USE_TZCNT -DLUA_USE_POSIX=1 -DNDEBUG -DSDL2=1 -DWICKED_CMAKE_BUILD -DWI_UNORDERED_MAP_TYPE=2 -D_GLIBCXX_USE_CXX11_ABI=1 -D_XM_AVX_INTRINSICS_ -D_XM_F16C_INTRINSICS_ -D_XM_FMA3_INTRINSICS_ -D_XM_SSE4_INTRINSICS_ -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/src -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/include -isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -isystem /usr/include/SDL2 -O3 -DNDEBUG -std=c++17 -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-sign-compare -fno-exceptions -fno-rtti -fno-stack-protector -fcf-protection=none -fno-stack-clash-protection -fno-stack-check -fno-asynchronous-unwind-tables -Wno-strict-aliasing -mavx -mpopcnt -mlzcnt -mbmi -mf16c -mfma -mfpmath=sse -pthread -Winvalid-pch -include /media/s2000/workspaces/wicked_engine/build/Samples/Tests/CMakeFiles/Tests.dir/cmake_pch.hxx -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -O3 -DNDEBUG -MD -MT CMakeFiles/wicked_engine_app.dir/main.cpp.o -MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d -o CMakeFiles/wicked_engine_app.dir/main.cpp.o -c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp
```

Compile successful.
Is it enough to pass just `-DSDL2=1`?
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ /usr/bin/c++  -DSDL2=1 -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -O3 -DNDEBUG -MD -MT CMakeFiles/wicked_engine_app.dir/main.cpp.o -MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d -o CMakeFiles/wicked_engine_app.dir/main.cpp.o -c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp
```

Fewer errors, now just `fatal error: SDL_vulkan.h`.
Where is that file?
```shell
➤ find /usr/include/ -iname SDL_vulkan.h
/usr/include/SDL2/SDL_vulkan.h
```

In `/usr/include/SDL2/`.
Let's add `-isystem` for that.
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ /usr/bin/c++  -DSDL2=1 -isystem /usr/include/SDL2 -I/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine -O3 -DNDEBUG -MD -MT CMakeFiles/wicked_engine_app.dir/main.cpp.o -MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d -o CMakeFiles/wicked_engine_app.dir/main.cpp.o -c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp
```

Now error on `error: inlining failed in call to ‘always_inline’ ‘__m128i _mm_cvtps_ph(__m128, int)’: target specific option mismatch`.

I will need to debug which flag fixes that later.

Or read the documentation, might also work.


# # Wicked Engine Documentation

[`WickedEngine-Documentation.md` @ github.com](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md)

It's the exact same text as something I though I read somewhere else, but it might have been this document.
In any case, the instructions are incomplete.

I think the important bit is that I should not add the include directory and library linking as described in [Building And Linking](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md#building-and-linking).
I think I should figure out how to use a proper CMake import, or whatever it's called.
Does Wicked Engine come with any helpers for CMake consumers?
There are
- `./cmake/WickedEngineTargets.cmake`
- `./cmake/install/WickedEngineConfig.cmake`
- `./cmake/WickedEngineConfig.cmake`

that all seem relevant
So what do I do?
I need to tell CMake about the Wicked Engine `cmake`  directory.
How do I do that?
I don't know enough about CMake it seems.

Does Wicked Engine have an install build target?
Running `cmake-gui` and setting `CMAKE_INSTALL_PREFIX` to `../installed`.
```shell
/media/s2000/workspaces/wicked_engine/build
➤ cmake --build  . -j --target install
```
It seems to be rebuilding everything.
Why?

It produced a seemingly very well-behaved installation.
```shell
/media/s2000/workspaces/wicked_engine/installed
➤ find . -maxdepth 2
./include
./include/WickedEngine
./lib
./lib/WickedEngine
./lib/cmake
➤ find lib -maxdepth 2
lib/WickedEngine
lib/WickedEngine/(A bunch of .a files and a .so file.)
lib/WickedEngine/Content
lib/cmake
lib/cmake/WickedEngine
```

Now I need to learn how to use this library with CMake, since simply pointing CMake to the include directory and the `libWickedEngine.a` library file wasn't enough.
I'm pretty sure I need to add `/media/s2000/workspaces/wicked_engine/installed` to a search path somewhere.
Reading my notes in [[CMake - Third-Party Dependencies]].

# CMake - Third-Party Dependencies

Pre-built library packages are found with `find_package`.
For `find_package` to work either a `FindPACKAGE.cmake`, `PACKAGEConfig.cmake`, or `PACKAGE-config.cmake` file must be available.
We have
- `WickedEngineConfig.cmake`
- `WickedEngineTargets-release.cmake`
- `WickedEngineTargets.cmake`

`WickedEngineConfig.cmake` matches `PACKAGEConfig.cmake`, so we're good.
I'm not sure what the `.*Targets(-relese)?\.cmake` files are for.

CMake uses `CMAKE_MODULE_PATH` to decide where to look for `.cmake` files.
We can add paths to `CMAKE_MODULE_PATH` with
```cmake
list(APPEND CMAKE_MODULE_PATH "LIBRARY_INSTALL_DIR/lib/cmake")
```

Let's try that.

How do I know which library targets `WickedEngineConfig.cmake` provides?
Should I read the file?
Use what the Wicked Engine Linux sample say I should use?

My consumer application's `CMakeLists.txt` is now:
```cmake
cmake_minimum_required(VERSION 4.0)
project(wicked_engine_consumer)

# Find Wicked Engine.
list(
    APPEND
    CMAKE_MODULE_PATH
    "/media/s2000/workspaces/wicked_engine/installed/lib/cmake/WickedEngine"
)

find_package(WickedEngine REQUIRED)

add_executable(wicked_engine_app main.cpp)
target_link_libraries(
    wicked_engine_app
    PRIVATE
        WickedEngine::WickedEngine
)
```

Let's try it.

```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake .
CMake Error at CMakeLists.txt:11 (find_package):
  By not providing "FindWickedEngine.cmake" in CMAKE_MODULE_PATH this project
  has asked CMake to find a package configuration file provided by
  "WickedEngine", but CMake did not find one.

  Could not find a package configuration file provided by "WickedEngine" with
  any of the following names:

    WickedEngineConfig.cmake
    wickedengine-config.cmake

  Add the installation prefix of "WickedEngine" to CMAKE_PREFIX_PATH or set
  "WickedEngine_DIR" to a directory containing one of the above files.  If
  "WickedEngine" provides a separate development package or SDK, be sure it
  has been installed.
```

That didn't work.
Is it searching for the exact file name that we have?
```shell
/media/s2000/workspaces/wicked_engine/installed
➤ find . -iname WickedEngineConfig.cmake
./lib/cmake/WickedEngine/WickedEngineConfig.cmake
```
Yep, that file exists.

Is the path I passed to `CMAKE_MODULE_PATH` wrong?
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ ls -1 /media/s2000/workspaces/wicked_engine/installed/lib/cmake/WickedEngine
WickedEngineConfig.cmake
WickedEngineTargets-release.cmake
WickedEngineTargets.cmake
```
The path is correct.

The error message says that it may not be `CMAKE_MODULE_PATH` we should add the path to, but actually `CMAKE_PREFIX_PATH`.
It seems `CMAKE_MODULE_PATH` is for `FindPACKAGE.cmake`, while `CMAKE_PREFIX_PATH` is for `PACKAGEConfig.cmake`.
Adding the Wicked Engine `lib/cmake` install path to `CMAKE_PREFIX_PATH` instead of `CMAKE_MODULE_PATH`.
Is `CMAKE_PREFIX_PATH` as list that can be appended just like `CMAKE_MODULE_PATH`?
Reading CMake documentation.

- [`CMAKE_MODULE_PATH` @ cmake.org](https://cmake.org/cmake/help/latest/variable/CMAKE_MODULE_PATH.html):

> [Semicolon-separated list](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#cmake-language-lists) of directories, represented using forward slashes, specifying a search path for CMake modules to be loaded by the [`include()`](https://cmake.org/cmake/help/latest/command/include.html#command:include "include") or [`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package "find_package") commands before checking the default modules that come with CMake.


- [`CMAKE_PREFIX_PATH` @ cmake.org](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html):

> [Semicolon-separated list](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#cmake-language-lists) of directories specifying installation _prefixes_ to be searched by the [`find_package()`](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package "find_package"), [`find_program()`](https://cmake.org/cmake/help/latest/command/find_program.html#command:find_program "find_program"), [`find_library()`](https://cmake.org/cmake/help/latest/command/find_library.html#command:find_library "find_library"), [`find_file()`](https://cmake.org/cmake/help/latest/command/find_file.html#command:find_file "find_file"), and [`find_path()`](https://cmake.org/cmake/help/latest/command/find_path.html#command:find_path "find_path") commands.


So both are semicolon-separate lists and both are used by `find_package`.
So it makes sense to try and use `CMAKE_PREFIX_PATH` now that `CMAKE_MODULE_PATH` has been tested and failed.

Let's ask ChatGPT what it thinks about these two variables.
- `CMAKE_MODULE_PATH` is for `FindPACKAGE.cmake`.
- `CMAKE_PREFIX_PATH` is for `PACKAGEConfig.cmake`.
- `CMAKE_PREFIX_PATH` should point to the root of the library installation, not to the directory containing `PACKAGEConfig.cmake`.

So everything seems to make sense.

Updating my `CMakeLists.txt` to have
```cmake
list(
    APPEND
    CMAKE_PREFIX_PATH
    "/media/s2000/workspaces/wicked_engine/installed/"
)
```

Works.

Build the application.
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ make -j
[ 50%] Building CXX object CMakeFiles/wicked_engine_app.dir/main.cpp.o
In file included from /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/wiAudio.h:3,
                 from /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/wiResourceManager.h:4,
                 from /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/wiApplication.h:4,
                 from /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/WickedEngine.h:13,
                 from /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp:2:
/media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/wiMath.h:29:10: fatal error: Utility/DirectXMath/DirectXMath.h: No such file or directory
   29 | #include "Utility/DirectXMath/DirectXMath.h"
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
make[2]: *** [CMakeFiles/wicked_engine_app.dir/build.make:79: CMakeFiles/wicked_engine_app.dir/main.cpp.o] Error 1
make[1]: *** [CMakeFiles/Makefile2:87: CMakeFiles/wicked_engine_app.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
```

I think that is a Wicked Engine file and not an actual DirectX file.
```shell
/media/s2000/workspaces/wicked_engine/installed
➤ find . -iname DirectXMath.h
./include/WickedEngine/Utility/DirectXMath.h
```
The file exists.

What include paths did `WickedEngine::WickedEngine` give us?
```shell
/media/s2000/workspaces/wicked_engine_app/build[2]
➤ make -j1 VERBOSE=1
[ 50%] Building CXX object CMakeFiles/wicked_engine_app.dir/main.cpp.o
/usr/bin/c++ \
	-DJPH_USE_AVX \
	-DJPH_USE_F16C \
	-DJPH_USE_FMADD \
	-DJPH_USE_LZCNT \
	-DJPH_USE_SSE4_1 \
	-DJPH_USE_SSE4_2 \
	-DJPH_USE_TZCNT \
	-DLUA_USE_POSIX=1 \
	-DNDEBUG \
	-DSDL2=1 \
	-DWICKED_CMAKE_BUILD \
	-DWI_UNORDERED_MAP_TYPE=2 \
	-DWickedEngine_SHADER_DIR=\"../installed/include/WickedEngine/shaders/\" \
	-D_XM_AVX_INTRINSICS_ \
	-D_XM_F16C_INTRINSICS_ \
	-D_XM_FMA3_INTRINSICS_ \
	-D_XM_SSE4_INTRINSICS_ \
	-isystem /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine \ <<< This one should find it.
	-isystem /usr/include/SDL2 \
	-isystem /media/s2000/workspaces/wicked_engine/installed/include \
	-isystem /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/FAudio \
	-O3 \
	-DNDEBUG \
	-mavx \
	-mpopcnt \
	-mlzcnt \
	-mbmi \
	-mf16c \
	-mfma \
	-mfpmath=sse \
	-pthread \
	-MD \
	-MT CMakeFiles/wicked_engine_app.dir/main.cpp.o \
	-MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d \
	-o CMakeFiles/wicked_engine_app.dir/main.cpp.o \
	-c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp
```

Make sure the path is correct my copy-paste concatenating the marked `-isystem` path and the `#include` path from the error message.
```shell
/media/s2000/workspaces/wicked_engine/installed
➤ ls /media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/Utility/DirectXMath/DirectXMath.h
ls: cannot access '/media/s2000/workspaces/wicked_engine/installed/include/WickedEngine/Utility/DirectXMath/DirectXMath.h': No such file or directory
```
It doesn't exist.
But `find` found it.
What's going on?
The directory structure is different.
The `#include` is looking for `Utility/DirectXMath/DirectXMath.h`
The file actually exists at `Utility/DirectXMath.h`.
There is a difference between the Git repository and the installed package:
```shell
/media/s2000/workspaces/wicked_engine
➤ find . -iname DirectXMath.h
./installed/include/WickedEngine/Utility/DirectXMath.h
./working_copy/WickedEngine/Utility/DirectXMath/DirectXMath.h
```

It seems the install placed some header files in the wrong place.

I created a [support question](https://github.com/turanszkij/WickedEngine/issues/1609).

The conclusion seems to be that install is broken and we should create our example in-engine.
That is, Wicked Engine is not a library, it is a monolithic project.
That's fine, if that is what they want, but not what I need.


# Use Build Directory As Install

Instead of pointing to the install directory, can I build my application against the entire build tree?
Do we have `WickedEngineConfig.cmake` there as well?

```shell
/media/s2000/workspaces/wicked_engine/build
➤ find . -iname "WickedEngineConfig.cmake"
./cmake/install/WickedEngineConfig.cmake
./cmake/WickedEngineConfig.cmake
```

Yes, but not in a place where CMake expects to find it.
CMake needs `CMAKE_PREFIX_PATH/lib/cmake/PACKAGE/PACKAGEConfig.cmake`.
There is no path we can add to `CMAKE_PREFIX_PATH` that makes it find `WickedEngineConfig.cmake`.
Let's see if CMake figures it out anyway through sheer recursive search.

```shell
/media/s2000/workspaces/wicked_engine_app/working_copy
➤ cmake --fresh .
# Snip.
-- Build files have been written to: /media/s2000/workspaces/wicked_engine_app/working_copy
```

It did find it.
How?
Let's try with `find_package` debugging enabled.
```shell
/media/s2000/workspaces/wicked_engine_app/working_copy
➤ cmake --fresh . --debug-find-pkg=WickedEngine
```

CMake searches in `CMAKE_PREFIX_PATH/cmake`, which is where the file is.

Let's try a build.
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake --build .
```

Success.
And I can run it as well.
I now have a working setup that can build an application that includes `WickedEngine.h`.
Let's try and call a few functions.


# Smallest Possible Wicked Engine Application

Well, maybe not smallest possible.
I'm using the C++ code from the [Linux template](https://github.com/turanszkij/WickedEngine/blob/master/Samples/Template_Linux/main.cpp).

It builds, but cannot run.
Getting Vulkan errors at start-up.
I think the problem is that it needs the shader compiler library, i.e. `libdxcompiler.so`.
That bit is handled by the Linux template's `CMakeLists.txt`.
Somewhat reduced to work for my case, added to my `CMakeLists.txt`:
```cmake
set(LIB_DXCOMPILER "libdxcompiler.so")
set(LIBDXCOMPILER_PATH "${WICKED_ROOT_DIR}/WickedEngine/${LIB_DXCOMPILER}")
add_custom_command(
    TARGET
        wicked_engine_app
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different ${LIBDXCOMPILER_PATH} ${CMAKE_CURRENT_BINARY_DIR}
)
```

Now the application can launch properly, it compiles a bunch of shaders, and then I get a black window with some text in the corner.
Wicked Engine version, frame buffer size, and frame rate.
Just like the `Template_Linux` sample executable in the engine.

So the basics are in place.
Now what?
What do I want to render?
My goal is to be able to create stuff dynamically from within the application.
So I want a Spawn Sphere button.
That's a good next goal.
But to get there I need to study some documentation.


# [`WickedEngine-Documentation.md`](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md)

I have learned how to load a compiled model,
```cpp
wi::scene::LoadModel
```
bu the window is still just black.
I think the [Sponza](https://upload.wikimedia.org/wikipedia/commons/thumb/a/ad/Crytek_Sponza.png/960px-Crytek_Sponza.png) scene include light sources.

It seems component types must implement the `Serialize` function even though the documentation says it shouldn't need to.

> It supports serialization of this data, and if this is used, then the component structs must have a Serialize() function. Otherwise the component can be any c++ struct that can be moved.


```
/WickedEngine/wiECS.h:241:51: error: ‘struct MyComponent’ has no member named ‘Serialize’
  241 |                                         component.Serialize(archive, seri);
      |                                         ~~~~~~~~~~^~~~~~~~~
```

I asked on the [Discord `help` forum](https://discord.com/channels/602811659224088577/1509277852271640676).
This resulted in a [pull request](https://github.com/turanszkij/WickedEngine/pull/1625) that has been merged to `master`.
Let's update my engine and try my example again.
Let's remind ourselves of the build steps by reading earlier in this note.
I think this should work:
```shell
/media/s2000/workspaces/wicked_engine/working_copy
⎇  master➤ mn.git_pull_and_log.fish
# Snip.
 e6c33359 Dennis Brakhane 2026-05-28T07:20:01+02:00  make Serialize optional for Components (#1625)
# Snip.

/media/s2000/workspaces/wicked_engine/build
➤ cmake --build . -j

/media/s2000/workspaces/wicked_engine_app/build
➤ cmake --build . -j
```

Works.

Sanity-check, calling `Serilize` on the `MyComponent` Component Manager should cause a runtime error.
```cpp
std::cout << "Serializing\n";
wi::Archive archive;
wi::ecs::EntitySerializer serializer;
components.Serialize(archive, serializer);
std::cout << "Serialized\n";
```

It did not.
Because the "runtime error" is `assert(0);`, and I built the engine in release which means no asserts.


Well, one road-block cleared, let's continue reading [WickedEngine-Documentation.md](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md).

# `WickedEngine-Documentation.md` > [Graphics](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md#graphics)

There is a lot of text here, about things I don't really understand.
I'm I supposed to do all of this, or is it describing what the engine is doing all by itself?
It talks about command queues, swap-chains, resource bindings, pipeline states, and render passes.

I need a tutorial.

# Unofficial Wicked Engine Wiki > Getting Started

Maybe this is a more user-friendly resource.
A few sections in this already feel way more useful than [`Wickedengine-Documentation.md`](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md).
It says what I need to do, instead of listing classes and functions in some arbitrary order.
We're doing stuff with `GameRenderPath` and `GameApp`.

The tutorial says I should be able to build and run now, and that this should be enough to have things appear on screen.

# Linker Errors Due To Missing `typeinfo`

Unfortunately, the build fails:
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake --build . -j ;and ./wicked_engine_app
[ 50%] Building CXX object CMakeFiles/wicked_engine_app.dir/main.cpp.o
[100%] Linking CXX executable wicked_engine_app

/usr/bin/ld:
CMakeFiles/wicked_engine_app.dir/main.cpp.o:
(.data.rel.ro._ZTI7GameApp[_ZTI7GameApp]+0x10):
undefined reference to 'typeinfo for wi::Application'

/usr/bin/ld:
CMakeFiles/wicked_engine_app.dir/main.cpp.o:
(.data.rel.ro._ZTI14GameRenderPath[_ZTI14GameRenderPath]+0x10):
undefined reference to 'typeinfo for wi::RenderPath3D'
```

That's a problem.
I recognize this error.
It can happen when the base class is built with `-fno-rtti` but the application is not.
It means that the classes we declared, `GameApp`, and `GameRenderPath`, are trying to generate RTTI information for themselves and to do that they needs to find the RTTI information for their base classes, which doesn't have any RTTI since then engine was compiled with `-fno-rtti`.

Let's do a verbose build to see if the application source files are being built with or without `-fno-rtti`.
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake --build . --target clean
➤ make -j1 VERBOSE=1

[ 50%] Building CXX object CMakeFiles/wicked_engine_app.dir/main.cpp.o
/usr/bin/c++ \
	-DJPH_USE_AVX \
	-DJPH_USE_F16C \
	-DJPH_USE_FMADD \
	-DJPH_USE_LZCNT \
	-DJPH_USE_SSE4_1 \
	-DJPH_USE_SSE4_2 \
	-DJPH_USE_TZCNT \
	-DLUA_USE_POSIX=1 \
	-DNDEBUG -DSDL2=1 \
	-DWICKED_CMAKE_BUILD \
	-DWI_UNORDERED_MAP_TYPE=2 \
	-DWickedEngine_SHADER_DIR=\"/media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/shaders/\" \
	-D_DEBUG \
	-D_XM_AVX_INTRINSICS_ \
	-D_XM_F16C_INTRINSICS_ \
	-D_XM_FMA3_INTRINSICS_ \
	-D_XM_SSE4_INTRINSICS_ \
	-isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine \
	-isystem /usr/include/SDL2 \
	-isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/src \
	-isystem /media/s2000/workspaces/wicked_engine/working_copy/WickedEngine/Utility/FAudio/include \
	-g \
	-mavx \
	-mpopcnt \
	-mlzcnt \
	-mbmi \
	-mf16c \
	-mfma \
	-mfpmath=sse \
	-pthread \
	-MD \
	-MT CMakeFiles/wicked_engine_app.dir/main.cpp.o \
	-MF CMakeFiles/wicked_engine_app.dir/main.cpp.o.d -\
	o CMakeFiles/wicked_engine_app.dir/main.cpp.o \
	-c /media/s2000/workspaces/wicked_engine_app/working_copy/main.cpp

[100%] Linking CXX executable wicked_engine_app
/usr/bin/c++ \
	-g \
	-pthread \
	-Wl,--dependency-file=CMakeFiles/wicked_engine_app.dir/link.d \
	CMakeFiles/wicked_engine_app.dir/main.cpp.o \
	-o wicked_engine_app  \
	/media/s2000/workspaces/wicked_engine/build/WickedEngine/libWickedEngine.a \
	-ldl /media/s2000/workspaces/wicked_engine/build/WickedEngine/libJolt.a \
	/media/s2000/workspaces/wicked_engine/build/WickedEngine/LUA/libLUA.a \
	/media/s2000/workspaces/wicked_engine/build/WickedEngine/Utility/libUtility.a \
	/media/s2000/workspaces/wicked_engine/build/WickedEngine/Utility/FAudio/libFAudio.a \
	/usr/lib/x86_64-linux-gnu/libSDL2.so
```

No `-fno-rtti` here.
I think the engine is built with `-fno-rtti` since I saw that flag in the `Test.cpp` build earlier.
It is surprising that the the flag is not propagated to my build by CMake.
```cmake
target_link_libraries(
    wicked_engine_app
    PRIVATE
        WickedEngine::WickedEngine
)
```

I think it should be set by `WickedEngineTargets.cmake`.
It is set the engine's root `CMakeLists`.
```cmake
if (MSVC)
	# Windows flags.
else()
	add_compile_options(
		$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<BOOL:${ubsan_active}>>,$<NOT:$<BOOL:${WICKED_ENABLE_RTTI}>>>:-fno-rtti> # runtime type information disabled
	)
endif()
```

Since this is set as a directory setting and not a target setting it won't propagate to `WickedEngineTargets.cmake`.

The expression is quite a mouthful, let's decode it.
```cmake
$<
	$<
		AND:
			$<
				COMPILE_LANGUAGE:CXX
			>,
			$<
				NOT:
					$<
						BOOL:
							${ubsan_active}
					>
			>,
			$<
				NOT:
					$<
						BOOL:
							${WICKED_ENABLE_RTTI}
					>
			>
		>
		:
		-fno-rtti
>
```

I'm not 100% sure, but I read this as
```cpp
if (COMPILE_LANGUAGE == CXX && !ubsan_active && !WICKED_ENABLE_RIIT)
{
	return "-fno-rtti"
}
```


I don't think I can replicate that logic in my `CMakeLists.txt`, I don't think `WickedEngineConfig.cmake` propagates `ubsan_active` and `WICKED_ENBLE_RTTI`, but I can at least hard-code the flag.
```cmake
target_compile_options(wicked_engine_app PRIVATE "-fno-rtti")
```

Let's attempt another build.
```shell
/media/s2000/workspaces/wicked_engine_app/build
➤ cmake --build . -j
➤ ./wicked_engine_app
```

Works.
And since I already experimented with `LoadModel` I get the Sponza scene :D

# Continue Unofficial Wicked Engine Wiki > Getting Started

I'm learning about accessing the camera, transformations, finding entities, and getting components.

Some relevant code snippets.

Configure Wicked Engine for 3D rendering.
```cpp
class GameRenderPath : public wi::RenderPath3D
{
    public:
        void Load() override;
        void Update(float dt) override;
};

void GameRenderPath::Load()
{
    RenderPath3D::Load();
}

void GameRenderPath::Update(float dt)
{
    RenderPath3D::Update(dt);
}

class GameApp : public wi::Application
{
    public:
        GameRenderPath renderer;
        void Initialize() override;
};

void GameApp::Initialize()
{
    wi::Application::Initialize();
    renderer.init(canvas);
    renderer.Load();
    ActivatePath(&renderer);
}
```

Set camera transformation.
```cpp
wi::scene::TransformComponent transform;
transform.Translate(XMFLOAT3(0.0f, 1.5f, -4.0f));
transform.UpdateTransform();
wi::scene::GetCamera().TransformCamera(transform);
```

Load a model.
```cpp
const std::string model_dir = "/media/s2000/workspaces/wicked_engine/working_copy/Content/models/";
const std::string model = model_dir + "teapot.wiscene";
wi::ecs::Entity model_entity = wi::scene::LoadModel(model, XMMatrixIdentity(), true);
```

Move an entity.
```cpp
wi::ecs::Entity lid = wi::scene::GetScene().Entity_FindByName("Top");
wi::scene::TransformComponent* lid_transform = wi::scene::GetScene().transforms.GetComponent(lid);
lid_transform->Translate(XMFLOAT3(0.0f, 0.6f, 0.0f));
```


# Inspecting A Scene

My goal is to be able to create entities and components programmatically, not just instantiate entire levels from a `.wiscene`.
To be able to do this I think it will be helpful to be able to inspect the game state after importing a model so that I can recreate it, with the tweaks I need.

The container for the scene contents seems to be `wi::scene::GetScene`.
This is what the Getting Started tutorial used to look up things.
So what we can do with whatever `wi::scene::GetScene` returns?
Unsurprisingly, `wi::scene::GetScene` returns a `wi::scene::Scene`.
It is declared in `WickedEngine/wiScene.h`.
It has a `wi::ecs::ComponentLibrary componentLibrary`.
What is a Component Library?
The class is declared in `WickedEngine/wiECS.h`.
The documentation says
> This is the class to store all component managers,
>  this is useful for bulk operation of all attached components within an entity

It is basically a String → Component Manager map.
The String is a hard-code component type-name  in `wi::scene::Scene`.
A Component Manager is a basically a Entity → `typename T` map.
The `wi::scene::Scene` definition has a bunch of variants of
```cpp
wi::ecs::ComponentManager<NameComponent>& names = componentLibrary.Register<NameComponent>("wi::scene::Scene::names");
```

Are these components everything needed to render a scene?

Let's read `wi::scene::LoadModel`, just to see what it does.

## `wi::scene::LoadModel`

There are some overloads and parameter creation and eventually we end up in`LoadModel2` which is where something happens.
- Open a `wi::Archive`.
- Serialize the archive into the scene.
	- This is the bit I may want to inspect further, but not right now.
- Create a root entity, if necessary.
- Attach  all non-attached loaded entities to the root entity.
- The root entity is transformed to where it should go.
- The root entity is destroyed, if necessary.


There is a bunch of helper `Create.+` functions in `wiScene.cpp`.
Those will be helpful, I think.
I should study them next.

All entity-creation is done with the global `wi::ecs::CreateEntity`, which is nothing more than a `std::atomic<uint64_t>.fetch_add`.
Removing an entity seems to be more complicated.
It is done by `wi::scene::Scene::Entity_Remove`.
It can remove attached children recursively.
The actual remove is a loop over the `wi::ecs::ComponentLibrary`, calling `Remove(Entity)` on each `wi::ecs::ComponentManager`.

So an Entity is a global thing, but very small.
It is just a counter, nothing else.
The data is all stored within `wi::ecs::ComponentManager` instances owned by a `wi::ecs::ComponentLibrary`.
So how do I get a list of entities within a scene?
There is `wi::scene::FindAllEntities`.
I assume that does an exhaustive search.
I'll be happy if it does something cleverer, which would mean that I have something to learn.
It does exhaustive search.
Basically.
```cpp
Set<Entity> entities;
for (ComponentManager& manager : componentLibrary)
{
	entities.insert(manager.GetEntityArray);
}
```


Reading a bit in `wiScene.h`.
It has a bunch of rendering related stuff.
Seems weird to have in that class.


## /

I think we have what we need to inspect a scene.
Let's print everything that seem even remotely possible of being populated after loading `teapot.wiscene`.
We'll use `wi::scene::FindAllEntities` to get a list of entities and the query some of the `wi::ecs::ComponentManager` entities to see what components we have.


I've been doing a bit of experimentation and I can create both lights and cubes.
The cubes even at runtime.
Though it doesn't seem to do any sharing of GPU resources between the cubes, which may be a problem.

I'm trying to understand how to use the vector types, but they seem very... limited.
Not sure what the best word to use is.
There are multiple types, no easy way to convert between them, and each operation seem to be  using a separate type.
- `XMVECTOR`: A 4-float SIMD register.
- `XMVECTORF32`: A union between a `float[4]` and a `XMVECTOR`.
- `XMFLOAT3`: `x`, `y`, `z` struct.

Wicked Engine is mostly using `XMFLOAT3`.
Operators are on `XMVECTOR`.
I'm trying to understand how to place cubes in a circle.
I want this:
```cpp
float radius {5.0f};
Vec3 direction {sin(t), 0.0f, cos(t)};
Vec3 position = radius * direction;
transform->Translate(position);
```



# References

- 1: [_Wicked Engine Documentation_](https://github.com/turanszkij/WickedEngine/blob/master/Content/Documentation/WickedEngine-Documentation.md)
- 2: [_Unofficial Wicked Engine Wiki_ > _Getting Started_  @ wicked-wiki.github.io](https://wicked-wiki.github.io/getstarted/)
