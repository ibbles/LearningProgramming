A performance blocker is a programming technique or software design that prevents the program from reaching the performance capability of the machine.
It is the opposite of a performance enhancer.

# Pointer Chasing

Data structures that result in implementations that rely on pointer chasing often leads to poor utilization of both compute cores and the memory buss.
Pointer chasing means that a sequence of reads all depend on each other.
That is, each read produces the address of the next read.
This creates a situation where the compute core cannot do useful work since it is stuck waiting for the reads to provide it the data it needs, and the memory buss cannot do useful work because it is only given infrequent requests that cannot be overlapped.
There is no way to do latency hiding and the program as a whole becomes limited by the round-trip time of the memory, which is way slower than something that is memory bandwidth limited.

Typical example of such data structures are linked lists and trees.
There exists variants of these that reduce the amount of pointer chasing required and increases the amount of valuable data fetch for each read operation.
For example, B-trees and B*-trees store more elements per node in the tree so that each read operation gives us multiple elements at once and let the compute cores do more work before it must block and wait for another memory read to complete.
For example, instead of using a heap allocation per element in a linked list we can allocate a larger block of memory that holds many elements close to each other and we can sort, out-right or incrementally, the elements so that when iterating over the linked list we walk from the start of the memory block towards the end.

A similar problem happens in an object oriented application where objects are made up of other objects that are separately allocated and accessed through pointers.
You get a network of linked objects that are scattered all over memory and expressions such as `car->getTransmission()->getEngine()->getTorqueCurve()->getCubicInterpolator()->evaluate(400_rpm)` results in a stall on every `->`.
This is why some people call `->` the cache miss operator.

# False Sharing

False sharing is when two values lie on the same cache line and one thread is modifying one of then while another thread is accessing the other [(1)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001070000000000000000).
The modify will invalidate the cache entry of the other thread.
So if you have a bunch of values that all are to be computed incrementally by a bunch of threads then don't put them close together such as in an array or `std::vector`.

One way to make sure the values aren't on the same cache line is to make their type cache line size aligned.
Then each instance will be placed on the start of the cache line and there is no risk of false sharing.
So we trade memory for speed.

Another way to avoid, or at least reduce, the false sharing that is possible in some cases is to do the computation on a local variable and only at the end of the algorithm write the final result to the shared memory location.

Another way to eliminate false sharing is to keep frequently updated data in memory regions that are reserved for a single thread, such as on the stack or in thread-local storage [(1)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001070000000000000000).

A rule of thumb that can help reduce false sharing is to keep values that are modified away from read-only values.
Typically most operations are read operations and if we fill a bunch of cache lines with just read-only values then those cache lines will never be invalidated.

# Pointer Aliasing

Pointer, or reference, aliasing is when two names refer to the same memory location [(1)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001040000000000000000).
That is, writing to one will change the value of the other .
If the compiler cannot prove that two pointer cannot alias then the value of one cannot be cached in a register across a write to the other and must be read from memory again.
This is true even if the programmer knows that the two pointers can never alias.

Consider the following example function.
```cpp
void translate(Vec3* positions, const size_t num, const Vec3& offset)
{
	for (size_t i = 0; i < num ;++i)
	{
		positions[i] += offset; // May write to 'offset'.
	}
}
```

If `translate` is called as follows
```cpp
Vec3* positions = allocate_positions(1024);
translate(positions, 1024, positions[10]);
```
then when `i = 10` in the loop we would reach the element that `offset` points to, so we do `offset += offset`, and all positions after that point will be moved twice as much as the preceding ones.
This is probably not what we intended, and we may even guarantee that we will never call `translate` like that, but the compiler cannot optimize based on our intentions.
So unless it can see the call site and optimize based on knowledge from there the compiler must be conservative and optimized based on the assumption that `offset` could alias `positions`.
It can optimize based on the assumption that no write in this function can change `num` for two reasons:
- `num` is passed by-value and no pointer or reference to it is created in the function.
- `num` is a `size_t` and there is no pointer or reference to a `size_t` or anything that contains a `size_t`.

We must be aware of the aliasing requirements of functions we call.
For example, `memcpy` does not allow the two memory areas to overlap, but `memmove` does allow them to.
In functions you write, be clear about what aliasing assumptions it makes.

Inlining can make it possible for the compiler to prove that at a particular call site there is no aliasing between two pointer / reference parameters and thus that particular instantiation of the function can be more aggressively optimized than the non-inlined instantiation.


# Stalled Cycles

A stalled cycle is one where the CPU can make no progress, i.e. not execute any instruction, because it is waiting for something [(1)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001090000000000000000).


# References

- 1: [_Data-Oriented Design_ > _Helping the compiler_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/node10.html)

