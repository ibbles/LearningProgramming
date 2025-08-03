# Thread Information

List the currently running threads with `info threads`.
```gdb
(gdb) info threads
```

The printout will include the name of each thread set with the `pthread_setname_np` function.

The currently selected thread is marked with `*`.

# Selected Thread

GDB always has exactly one selected thread.
Any context-specific commands such as `print` `list` and `backtrace` are done for the currently selected thread.

The selected thread is changed by [(1)](https://youtu.be/EJRgwWlqGRM?t=335):
- The `thread #` command.
- Another thread hitting a breakpoint or watchpoint.
- Another thread receiving a signal.


# Threads Stepping

By default GDB runs all threads when running any thread [(1)](https://youtu.be/EJRgwWlqGRM?t=361).
This means that if the application is stopped and you do `step` for one thread then all threads will advance as far as they can until the `step`'d thread is stopped again.
The other threads will often make more progress, execute more instructions, than the stepped thread since GDB also does some work while the other threads are still running.

To only run the selected thread turn on `scheduler-locking` [(1)](https://youtu.be/EJRgwWlqGRM?t=412):
```gdb
(gdb) set scheduler-locking on
```

With this the non-selected threads will not run when the selected thread is `step`'d or `continue`'d.
Beware that this will lead to a frozen application is the selected threads needs to take a lock that another thread already holds.
Hit CTRL+C to get back to the GDB prompt and step the thread holding to the lock to let the original thread take it.

An alternative value for `scheduler-locking` is `step` [(1)](https://youtu.be/EJRgwWlqGRM?t=570).
```gdb
(gdb) set scheduler-locking step
```

With this mode the `step` and `next` commands only step the current thread, but `continue` runs all threads.

To let the non-selected threads run in the background while the selected thread is stopped and the GDB terminal is active turn on `non-stop` mode [(1)](https://youtu.be/EJRgwWlqGRM?t=777).
`non-stop` mode must be turned on before the application is started.
```gdb
(gdb) set non-stop on
```

If the other threads do any output then that output may mess up the GDB console.
To avoid this start the application beforehand and attach to it from a separate terminal.

# Sending Commands To All Threads

Commands we type in the GDB console are by default passed only to the selected thread.
For example, if we do
```gdb
(gdb) print $pc
```
then the program counter for the current thread is printed.

To run a command for all threads use `thread apply` [(1)](https://youtu.be/EJRgwWlqGRM?t=669).
`thread apply` takes as its first argument which threads to send the command to and the rest of the arguments are the command itself.
The thread argument can be `all` for all threads or a list of thread IDs.
```gdb
(gdb) thread apply all print $pc
(gdb) thread apply 4-5 print $pc
```

This can be used to verify that a global variable is thread-local, since each thread will have that  variable at a different address.
```gdb
(gdb) thread apply all $errno
```

There is a short form of `thread apply all` named `taas` where the `s` means "silent", i.e. errors are suppressed.
```gdb
(gdb) taas print $errno
```



# Signals

It is not defined which thread will receive a signal.
When you hit CTRL+C in GDB the operating system will send the interrupt signal, `SIGINT`, to the application and any thread may receive it, meaning that any thread may become the selected one in GDB [(1)](https://youtu.be/EJRgwWlqGRM?t=137).
In practice it is usually thread 1, the main thread.


# References

- 1: [_Debugging C/C++ Threads with GDB & Pthreads | GDB Tutorial_ by Undo @ youtube.com 2025](https://www.youtube.com/watch?v=EJRgwWlqGRM)