# Print Variables

A function that prints variables available in the current scope [(1)](https://stackoverflow.com/a/9328525):
```cmake
function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if ((NOT DEFINED ARGV0) OR _variableName MATCHES ${ARGV0})
            message(STATUS "${_variableName}=${${_variableName}}")
        endif()
    endforeach()
endfunction()

# Example usage.
dump_cmake_variables("SomePrefix.*SomeSuffix")
```

The documentation says:

> List of variables defined in the current directory.

Not sure if that means that they became defined in the current directory, or if it also includes variables that were defined elsewhere, in a parent directory, but is available also in the current directory.
# References

- [_CMake: Print out all accessible variables in a script_ by sakra @ stackoverflow.com](https://stackoverflow.com/a/9328525)
- [_VARIABLES_ @ cmake.org](https://cmake.org/cmake/help/latest/prop_dir/VARIABLES.html)
