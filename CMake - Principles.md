[(1)](https://youtu.be/NDfTwOvWIao?t=1520)


# Write Less CMake

Keep it simple [(1)](https://youtu.be/NDfTwOvWIao?t=1521).
Avoid:
- Interoperability logic
	- Platform specific or package manager specific things.
- Algorithms
- Loops
- Setting CMake variables that should be user controlled, such as:
	- `CMAKE_BUILD_TYPE`
	- `CMAKE_CXX_STANDARD` (at least for libraries) [(1)](https://youtu.be/NDfTwOvWIao?t=1780)
		- Better if this is supplied by the consumer of the library.
		- Risk of ABI incompatibility if different libraries are built with different C++ standards.
		- We want all projects to build with the exact same `CMAKE_CXX_STANDARD`, which is difficult if each `CMakeLists.txt` sets this variable for themselves.
		- Instead put it in the file pointed to by  `CMAKE_TOOLCHAIN_FILE`, or in a top-level `CMAKE_CXX_FLAGS`.
		- Detect if `CMAKE_CXX_STANDARD` has not been set and raise an error.
Minimize:
- Variables
- Conditionals
Instead [(1)](https://youtu.be/NDfTwOvWIao?t=1625):
- Describe your project factually, things that are independent of where you build or run it.
	- What are you building.
	- How is it lied out in the source directory.
	- What are the dependencies.
	- Which target owns which source files.
- Have a `test` target even if it contains 0 tests [(1)](https://youtu.be/NDfTwOvWIao?t=1721).

It is called `CMake`**Lists**`.txt` for a reason, it should list things [(1)](https://youtu.be/NDfTwOvWIao?t=1647).
- Library dependencies.
- Source and header files.
- Tests.



# References

- [_CMake Doesn't Have to Be Painful | Simple Strategies That Work _ by Bret Brown, CppCon @ youtube.com 2025](https://youtu.be/NDfTwOvWIao) 
