There are a number of techniques than can be employed to produce programs that make efficient use of computer hardware and thus reach high [[Performance]].
These techniques are called [[Performance Enhancer]]s.

There are two major classes of applications based on what part of the hardware is limiting the speed of the application:
- Compute, or core, bound.
- Memory bound.

There are also other types of bottlenecks that an application can suffer from, such as drive bound, network bound, user input bound, rendering bound, and many more.
A family of those that involve the the program waiting for an input or output operation to complete is called IO bound.

A single application can be compute, or core, bound in some parts and memory bound in other parts.
Different optimization strategies apply for the different classes of bottlenecks.
- [[Optimizing Core Bound Applications]]
- [[Optimizing Memory Bound Applications]]

# Compute, Or Core, Bound

A program is said to be compute, or core, bound when the bottleneck is in the CPU itself [(1)](https://youtu.be/Fs_T070H9C8?t=202).
Such applications are computationally intensive, meaning they perform many arithmetic operations compared to the number of memory accesses.
The FLOPS-to-bytes ratio is high, assuming a floating-point heavy application.
Such applications' performance is limited by things like the CPU clock speed, utilization of the functional units, instruction level parallelism, branch prediction, register allocation / pressure, the number of cores, data parallelism, SIMD utilization, false sharing, thread synchronization, `TODO continue this list`.


# Memory Bound

A program is said to be memory bound when the speed of the program is limited by the amount of data it can transfer from the memory subsystem to the CPU [(1)](https://youtu.be/Fs_T070H9C8?t=219).
Such applications are memory intensive, meaning they perform few arithmetic operations compared to the number of memory accesses.
The FLOPS-to-bytes ratio is low, assuming a floating-point heavy application.
Such applications' performance is limited by things like memory bandwidth or latency, non-predictable memory access patterns / cache misses, insufficient prefetching, cache size or unfortunate eviction patterns, indirect references, pointer or index chasing, `TODO continue this list`.

# References

- 1: [_Introduction to Hardware Efficiency in Cpp - Ivica Bogosavljevic - CppCon 2022_ by Ivica Bogosavljevic, CppCon @ youtube.com 2022](https://www.youtube.com/watch?v=Fs_T070H9C8)
