GDB is one of the debuggers on Linux.


# Building Debuggable Applications

Pass the `-g` flag to `gcc` or `clang` to build with debug information included [(3)](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Compilation.html#Compilation).
This helps the debugger read and present the application state to the user.
This includes mappings from instruction addresses to source files and lines.

Compiler optimizations can hamper the debuggers ability to debug the application.
It is often best to compiler with `-O0` or `-Og`.

Example:
```shell
$ gcc -Og -g my_application.c -o my_application
```


# Launching GDB

Start GDB with
```shell
$ gdb ./my_application
```
where `my_application` is the application to be debugged.

This will load the application and the associated debug information.
It does not start executing the application.


# Running Or Stepping The Application

The `run` commands starts the application.
```gdb
(gdb) run
```

The application will run until a breakpoint or watchpoint is hit, a signal, such as `SIGINT` from CTRL+C, is received, or the application terminates.
(
Are the other things that causes execution to halt or terminate?
)

We need not run the application at full speed until something stops it.
We can also run until the end of the current function with `finish`, or one of its short forms.
```gdb
(gdb) finish
(gdb) fin
```

We can also run to the next line of code with the `next` command, or one of its short forms.
```gdb
(gdb) next
(gdb) n
```

`next` can take an argument that is the number of times to execute `next`, letting us step over multiple lines at once.
```gdb
(gdb) next 5
```

If the current line contains a function call then `next` will step over the function.
To step into the function use the `step` command.
```gdb
(gdb) step
(gdb) s
```

`step` can take an argument that is the number of times to execute `step`.
```gdb
(gdb) step 5
```

For even more granular stepping we can step a single machine instruction with `stepi`.
```gdb
(gdb) stepi
(gdb) stepi 5
(gdb) si
```

To let a loop run until completing use the `until` command [(4)](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html#Continuing-and-Stepping).
It will single-line-step just like `step` until the end of a loop iteration is reached, then it will fast-forward past all loop-iterations.

To not step into function calls when instruction-stepping, use `nexti` instead of `stepi`.
```gdb
(gdb) nexti
(gdb) nexti 5
(gdb) ni
```


# Displaying Data

Print a value once with `print`:
```gdb
(gdb) print my_variable
```

Display a value continuously, i.e. every time we stop, with `display`:
```gdb
(gdb) display my_variable
(gdb) disp my_variable
```
# References

- 1: [_GDB: The GNU Project Debugger_](https://www.sourceware.org/gdb/documentation/)
- 2: [_Debugging with GDB_](https://sourceware.org/gdb/current/onlinedocs/gdb.html/)
- 3: [_Debugging with GDB_ > _4.1 Compiling for Debugging_](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Compilation.html#Compilation)
- 4: [_Debugging with GDB_ > _5.2 Continue and Stepping_](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html#Continuing-and-Stepping)
