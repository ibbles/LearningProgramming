The purpose of profiling is to analyze the runtime behavior of a program, or part of a program, in order to identify bottlenecks and [[Performance Blocker]]s.
The end-goal is typically to improve [[Performance]].

When investigating the performance characteristics or a program or part of a program there are two types of bottlenecks [(1)](https://youtu.be/Fs_T070H9C8?t=194):
- compute, or core, bound.
- memory bound.

Different parts of a program can be compute bound or memory bound, and a program can be either compute bound or memory bound based on the input.

It is important to establish which of the two types of bottlenecks the program we are dealing has.


# References

- 1: [_Introduction to Hardware Efficiency in Cpp - Ivica Bogosavljevic - CppCon 2022_ by Ivica Bogosavljevic, CppCon @ youtube.com 2022](https://www.youtube.com/watch?v=Fs_T070H9C8)
