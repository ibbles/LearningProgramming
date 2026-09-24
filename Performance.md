By performance we here mean the speed at which a program can perform a task.
More performance means less time spent performing the task.
Performance have many facets, for example we may be aiming for high throughput (Bulk processing of many tasks.) or low latency (Short time between the submission of a task and it's completion.).

Many things lead to good performance.
For example:
- The language a program is written in can impact the program's performance [(1)](https://youtu.be/Fs_T070H9C8?t=90).
- Picking or designing a software architecture that is inducive to performance [(1)](https://youtu.be/Fs_T070H9C8?t=120).
- Avoiding unneeded work, or repeating work. 
- Use algorithms with low algorithmic complexity, but also not needlessly complicated algorithms for the task at hand.
- Good programming language best practice, such as using `std::move` and references where appropriate [(1)](https://youtu.be/Fs_T070H9C8?t=139).
- Use a high-quality compiler that has been suitable configured to produce optimized machine code for the application and target hardware [(1)](https://youtu.be/Fs_T070H9C8?t=148)
- Being hardware efficient by making efficient use of CPU, memory, drive, network, and so on [(1)](https://youtu.be/Fs_T070H9C8?t=156).


# Peak Performance

Peak performance is when a property of the hardware is saturated by the intrinsic complexity of the task, the amount of work that is inherent to the problem.
If a problem needs 1 billion arithmetic operations of compute and we have a CPU that can perform 1 billion arithmetic operations per second, and our program takes 1 second to complete, then we will not be able to make the program run faster.
It is often not obvious what the amount of work inherent to a particular problem is, it is not uncommon that a new more efficient algorithm for a problem is discovered or invented, reducing the amount of inherent work.

When we have reached peak performance then there is a computer hardware limitation that bottlenecks the runtime of the program.
There are two major characteristics of a computer that can limit the performance of a program [(1)](https://youtu.be/Fs_T070H9C8?t=194).
We say that a program is either
- compute, or core, bound
- memory bound.

To achieve peak performance we need to [[Optimization|Optimize]] our program.
There are different techniques for [[Optimizing Core Bound Applications]] and [[Optimizing Core Bound Applications]].


# References

- 1: [_Introduction to Hardware Efficiency in Cpp - Ivica Bogosavljevic - CppCon 2022_ by Ivica Bogosavljevic, CppCon @ youtube.com 2022](https://www.youtube.com/watch?v=Fs_T070H9C8)
