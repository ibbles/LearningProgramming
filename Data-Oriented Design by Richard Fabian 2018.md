# My Summary

Pointer aliasing prevents compiler optimizations.
Order data members so that data that is used together is placed together.


# Helping The Compiler

## Write Buffer Awareness

Similar rules as with reading.
Write contiguously.
Keep memory locations that are written to separate from read-only locations.
Do bulk write operations.

The cache can be bypassed.
Do this if you know that the value, or any surrounding value, won't be touched again for a while.
We don't want to pollute the cache, and evict useful data, needlessly.
This is called streaming operations.

Read [_What Every Programmer Should Know About Memory_ by Ulrich Drepper @ freebsd.org 2007](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf).
Streaming operations, i.e. those that bypass the cache, are called non-temporal memory operations in this text.


## Aliasing

Aliasing is when it is possible to refer to the same memory location using multiple names.
This can be through a pointer or a reference.
The possibility of aliasing means that values must be reloaded from memory whenever the aliasing name is written to.
A by-value parameter cannot be aliased, unless the function itself creates the alias.

Consider this piece of code that replicates a character over a buffer.
```cpp
char buffer[100];
buffer[0] = 'X';
memcpy(buffer+1, buffer, 98);  // (UB?)
buffer[99] = '\0';
```

This code relies on aliasing between the first and second parameter to `memcpy`.
(
I don't think this is allowed.
I think `memcpy` explicitly forbids this.
I think the author should have used `memmove` instead.
)
A write through the first parameter affects later reads through the second parameter.
The copy above only works if `memcpy` copies one byte at a time.
If it does SIMD reads and writes then we won't get a buffer filled with `X`s.

If the compiler has more knowledge about the surroundings of the possibly aliasing pointers / references then it can make more optimizations.
This is why inlining can have such a big impact sometimes.

## Return Value Optimization

Multiple return values can be done by having multiple multiple output reference parameters, by having an output reference parameter to a `struct`, or by returning a `struct` by value.
The return-by-value option is often good because the compiler can do return value optimization.
This means that the object to be returned is placed in the calling function's stack frame.
Value types are easier to reason about and easier for the compiler to optimize.

## Cache Line Utilization

Each memory access will read in a full cache line, which often contains more than just the data initially accessed.
A cache line is often 64 bytes.
Data that is on the same cache line as data recently accessed is likely to be very fast to access since it will likely be serviced from cache instead of main memory.

(
The author mentions a two-layer search in Chapter 6 - Searching.
I don't remember that, and cannot find it.
)

Organize the memory layout of you class so that the data that is accessed together is stored together.
That will increase the number of cache hits you get and thus improve performance.

A bitset is a very dense way of storing state information.
It can, for example, be used to store the `nullptr` / not `nullptr` state of a set of pointers in a large class.
That way we can very quickly determine if we even need to look at those pointers, and since many systems will look at the same bitset it is very likely to already be in cache.

A 64 byte cache line can hold one of:
- A 4x4 matrix of `float`s.
- 8 `double`s.
- 16 `int`s.
- 64 character ASCII string.


# False Sharing

False sharing is when two cores don't share any data, but the data they use are on the same cache line at at least one is a write.
In this case the two cores are invalidating each other caches and we get a bunch of inter-core traffic 
One way to combat this is to keep frequently updated values in memory that is only accessible from a single thread, such as on the stack or in thread-local storage.


# Speculative Execution Awareness

[(3)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001080000000000000000)

(
This entire section is written weirdly.
I'm having a hard time understanding it.
)

Speculative execution executes instructions ahead of time, before we know for sure that those instructions should be executed.
This can lead to vulnerabilities such as Spectre and Meltdown.

It is good to calculate the condition of a branch as early as possible.
It is good to structure you code so that code in branches has the smallest possible side-effect on the data.

A speculative load is still a load and can have a negative impact on overall runtime performance, especially when the load was speculated incorrectly and should not actually have happened.
It puts strain on the cache and memory subsystem [(4)](https://stackoverflow.com/questions/64141366/can-a-speculatively-executed-cpu-branch-contain-opcodes-that-access-ram).

Sometimes it is not possible to cache the actual value, but it is possible to cache whether a particular value exists or not.
For example, in complex data structures with multi-step look-ups to get at the actual data it can be beneficial to have a faster side-structure where the presence or absence of particular keys are recorded.
These side-structures can even have false-positives and still be beneficial.
Avoid false-negatives though, since that would change the behavior of the application .


## Branch Prediction

A common cause of a stalled CPU is not having any work to do.
(
That's pretty much the definition of a CPU stall....
)

Another cause is having to undo work done speculatively after an incorrect branch prediction.
This is especially bad if the speculatively executed code issued a memory load instruction since that wasted memory bandwidth and cache space.

Branch prediction is the act of deciding which code should be executed when we are forced to execute speculatively.
We can help the branch predictor.
Make branches consistently true or false in large batches.
One way to do this is to sort the data based on the the condition in the branch [(5)](https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array)
But sorting also comes with a cost, so make sure it is worth it. Measure.

Branching is expensive not because the jump is expensive, but because branch misprediction is expensive.
A vtable pointer, i.e. a virtual function call, is kinda like a branch in that it can go to difference locations.

# References

- 1: [_Data-Oriented Design_ > _Existential Processing_ by Richard Fabian, 2018]([https://www.dataorienteddesign.com/dodbook](https://www.dataorienteddesign.com/dodbook/node4.html))
- 2: [_What Every Programmer Should Know About Memory_ by Ulrich Drepper @ freebsd.org 2007](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
- 3: [_Data-Oriented Design_ > _Helping the Compiler_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/node10.html)
- 4: [_Can a speculatively executed CPU branch contain opcodes that access RAM?_ by golosovsky, Peter Cordes @ stackoverflow.com 2020](https://stackoverflow.com/questions/64141366/can-a-speculatively-executed-cpu-branch-contain-opcodes-that-access-ram)
- 5: [_Why is processing a sorted array faster than processing an unsorted array?_ by GManNickG, Mysticial @ stackoverflow.com 2012](https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array)
