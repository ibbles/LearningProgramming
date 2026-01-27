A performance enhancer is a programming technique or software design that helps the program reaching the performance capability of the machine.
It is the opposite of a performance blocker.

# Linear Memory Accesses

Access memory contiguously, in large amounts at a time, and use all the bytes [(1)](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf).

# Order Composite Type Members To Minimize Size And Alignment

Composite types have a size that depend on the members of the type and their order [(2)](https://youtu.be/IroPQ150F6c?t=704).
This is because all types have alignment requirements.
If the next byte doesn't have at least the same alignment as the next member then padding bytes will be inserted to reach the alignment requirement of the next member.
Also, to make arrays work the type as a whole must have a size that aligns with the alignment of the type so that when multiple instances are packed together in an array then each instance is properly aligned.
By reordering the members we can influence the size.
It is usually a good idea to order the members so that the size of the type is minimized.

Example:
```cpp
struct WithPadding
{
	int32_t a; // Start at byte 0, next byte is 4.
	// Padding bytes. Start at byte 4, next byte is 8.
	int64_t b; // Need 8-byte alignment, can't start at 4, must start at 8, next byte is 16.
	int32_t c; // Start at byte 16, next byte is 20.
	// However, can't start another instance at 20 becuase then 'int64 b' would start at 28,
	// which is not 8-byte aligned. So we get 4 bytes of padding here as well to get to 24,
	// which is 8-byte alligned.
}

struct WithoutPadding
{
	tin32_t a; // Start at byte 0, next byte is 4.
	int32_t c; // Start at byte 4, next byte is 8.
	int64_t b; // Start at byte 8, next byte is 16.
	// So now all members can start on the next byte after the previos members,
	// and the next instance in an array can start immediately after the 'b'
	// member. No padding bytes necessary.
}
```

With padding:
```
             1           2
 0 2  4 6  8 0 2 4  6 8  0 2  4
|aaaa|****|bbbbbbbb|cccc|****|
  1 3  5 7  9 1 3 5  7 9  1 3
              1           2
```

Without padding:
```
             1
 0 2  4 6  8 0 2 4  6
|aaaa|cccc|bbbbbbbb|
  1 3  5 7  9 1 3 5
              1
```

We have moved `c` into the padding between `a` and `b`, and allowed the next instance to start immediately after `b` at byte 16, with is a multiple of 8, the size in bytes of `in64 b`.


# Order Data Members So That Data Used Together Is Stored Together

This is mostly relevant for objects that are somewhat large, compared to the size of a cache line.
By ordering the data members so that data that is used together is placed together we get better use out of our cache.
After the initial read we get one cache line's worth of data for free.


# Replace `bool` Flags With Separation Into Multiple Containers

Adding a single `bool`, which is just 1 bit of information but requires 1 byte of storage, can add a lot more than that to the size of a composite type due to alignment requirements [(2)](https://youtu.be/IroPQ150F6c?t=765).

Example
```cpp
struct Entity
{
	int32_t a;
	int32_t b;
	int64_t c;
	bool alive;
	//   ^ This bool increases size of the struct
	// by 8 bytes due to alignment requirements of
	// 'int64_t c'.
}

std::vector<Entity> entities;
```

Another way to represent a true / false state is with existence / absence [(4)](https://www.dataorienteddesign.com/dodbook/node4.html#SECTION00450000000000000000).
If an object is in a particular container then it has some property, otherwise not [(2)](https://youtu.be/IroPQ150F6c?t=1008).
We can have one container with all the alive entities and another container with all the dead entities.

```cpp
struct Entity
{
	int32_t a;
	int32_t b;
	int64_t c;
}

std::vector<Entity> alive_entities;
std::vector<Entity> dead_entities;
```

This also makes it more efficient to perform operations that should only be done for instances that have or does not have that property since we can loop over just those instances [(2)](https://youtu.be/IroPQ150F6c?t=1094).
By not having to look at the alive flag we save a memory read and don't pollute the cache with cache lines we only needed a single byte from and then skipped because the entity was dead.
We also don't need to do a branch since we know that we should operate on all elements in the container.

A drawback is that we can only have one such property, unless we want to store the instances in many containers.


# Use Indices Instead Of Pointers

A pointer is a very general data handle, it can point to any address in memory.
This means that it must be large, 8 bytes on a 64-bit machine.
This means that if we store a bunch of pointers in a composite type then that type will also be large.
If we can allow ourselves to be more restrictive and less general then we can reduce the size of our composite type.
For example, if we store the indices into a container instead of pointer and limit the number of instances we can identify then we can use a smaller integer type to hold the index.

Though beware that using raw integer types for the indices reduces type safety since an index can be passed into any type of indexable container but a pointer can only be used to access a particular type. [(2)](https://youtu.be/IroPQ150F6c?t=933),
This means that a new class of bugs is made possible, which is bad.
Use a custom index type and a container type that supports custom index types to gain some type safety back.
Unfortunately C++ doesn't have strong typedefs.


# Split Composite Types Into Multiple Arrays, Struct-Of-Arrays

This is a technique that is useful when we have many instances of the same composite type and want to operate on subsets of their member variables efficiently.
Instead of having a composite type with all our members we store each member from every instance  in an array [(2)](https://youtu.be/IroPQ150F6c?t=1145).
This means that any padding that would be required in a composite type with the same members disappear and we can iterate over a subset of the members without polluting the cache with the other members.

```cpp
// Array-of-structs

struct Entity
{
	int32_t a;
	int32_t b;
	double c;
}

std::vector<Entity> entities;


// Struct-of-arrays.

struct Entities
{
	std::vector<int32_t> as;
	std::vector<int32_t> bs;
	std::vector<double> cs;
}

```

See also  [[Data-Oriented Design]].


# Store Sparse Data In Hash Maps, Store Optional Data Elsewhere

Sometimes you have a collection of objects where some member is optional, i.e. that member represents some sparse data.
Instead of storing the optional data directly as a by-value member we can replace the member with a pointer and store the data elsewhere.

Alternatively, we can opt to not store any member at all and instead use a lookup-table [(2)](https://youtu.be/IroPQ150F6c?t=1278).
An example where we have optimized for the case where most entities have one held item but some can have up to five.
```cpp
struct Entity
{
	// Other member variables.
	
	ItemId held_item;
};

std::vector<Entity> entities;
std::unordered_map<EntityId, ItemId[4]> held_items;
```


# Bypass The Cache When Only Touching A Cache Line Once

If you know that a particular value is only going to be accessed once, will not be accessed again in a while, and none of the other values on the same cache line will be accessed, then you can opt to bypass the cache [(5)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001030000000000000000).
This lets other data already in the cache and more likely to be used soon remain there.


# Inlining

Inlining can help with performance in many ways:
- No `call` instruction.
	- Which means no jump which means we don't need to start reading instructions from a completely different location. The instructions we are about to execute are thus more likely to already be in cache.
- If the function has possibly aliasing pointer / reference parameters then after inlining the compiler may be able to prove that the parameters do not alias in this case.
- The compiler can reorganize instructions from before, in, and after the function's code.
	- This can hide memory access latencies, make better use of the ALUs due to better instruction interleaving, and (what more?).
- If the call was inside a loop, the compiler may be able to unroll or even auto-vectorize the loop.


# Use A Bitset To Store Metadata About A Large Class

For large classes, larger than a cache line, it can be helpful for performance to have a bitset that store some metadata about the object [(5)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001060000000000000000).
The bitset can, for example, say whether a set of pointers are `nullptr` or not.
Then, by loading just that bitset, a function can know whether it even needs to load another part of the object or not.
And since many parts of the software will look at the bitset it is very likely to already be in cache.

A drawback is that it can introduce dependencies between systems that previously was independent.
If two systems previously only used separate parts of the objects, with the introduction of the bitset they now share a read/write memory location: the bitset.

# Speculative Execution

[(5)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001080000000000000000)

# Help The Branch Predictor

Make sure a branch is consistently true or false for a large contiguous chunk of iterations [(6)](https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array).


# Move correctness guarantees from runtime checks into data layout rules

Use the type system to express things like alignment and buffer size multiples, rather than runtime checks [(5)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001090000000000000000).
This gives the compiler more opportunities for optimization and cuts down on the amount of prologue / epilogue code needed around auto-vectorized SIMD loops.

```cpp
// f16 is a float type that is only allowed in buffers that are 16-byte aligned.
// Each individual float element in the buffer is not necessarily aligned, but we,
// as programmers, guarantee that any pointer we pass as an argument to a funtion
// will be 16-byte aligned.
typedef float f16 __attribute__((aligned(16)));

void Amplify(f16* a, float mult, int count)
{
	count &= -4;
	for (int i = 0; i < count; ++i)
	{
		a[i] *= mult;
	}
}
```

You can also use [`std::assume_aligned`](https://en.cppreference.com/w/cpp/memory/assume_aligned.html):
```cpp
void Amplify(float* a, float mult, int count)
{
	a = std::assume_aligned<16>(a);
	count &= -4;
	for (int i = 0; i < count; ++i)
	{
		a[i] *= mult;
	}
}
```

By:
- Allocating aligned buffers.
- Rounding counts upfront.
- Expressing guarantees in the type system.

You get:
- Simpler loops.
- Fewer instructions.
- Better I-cache density.
- More predictable performance.


# Help The Auto Vectorizer

Make sure the compiler can proves that buffers and iteration counts are aligned and the iteration count is known before the loop starts.

Use `buffer = std::assume_aligned<16>(buffer);` to tell the compiler that a buffer is aligned to 16 bytes.
Use `count &= -4` to tell the compiler that a count is a multiple of 4.
Use `alignas` to tell the compiler that a type or variable should be aligned.

Branching constructs such as `if (count % 4) return;` does seem to be as reliable as `count &= 4` to enable auto vectorization, though this is very much compiler version dependent.
GCC 13.2 (released 2023) is more picky with which variant of the `Amply` function above it vectorizes, while GCC 15.2 (released 2025) is more lenient and vectorizes more variants.
As always, measure and inspect your particular code and compiler combination.

Do not modify the iteration variable or do any `break`, `continue`, or `return` from within the loop body.
Also be wary of any loop that calls a function, that may prevent auto vectorization.
Inlined and `constexpr`/`consteval` functions are less prone to breaking auto vectorization.

The auto vectorizer is pretty good at handling branches within the loop body, but avoid conditional writes to memory.
The compiler cannot emit a SIMD memory store instruction if there is a possibility that some lanes in the SIMD registry should not be stored [(5)](https://www.dataorienteddesign.com/dodbook/node10.html).
There is no conditional store instruction.
(
What does Richard Fabian mean here?
There is `VPMASKMOV` that can write a subset of elements from a SIMD register to memory.
Though masked writes was improved quite a bit with AVX-512 regarding cache behavior, perhaps Richard is talking about pre AVX-512 code.
)

Some use `typedef float f16 __attribute__((aligned(16)));` to create a `float`-type to which a pointer must always be 16-byte aligned.
This syntax is weird to me.
Or rather it doesn't only say that a buffer of elements must be 16-bytes aligned but that _all_ elements of the buffer must be 16-bytes aligned.
Though that's not how the compiler interprets a pointer to an `f16`, so I guess it's fine.
But don't try to create an array of `f16`s, that won't work since in that case the compiler gets a contradiction between the type's size and alignment that it cannot satisfy, resulting in a compiler error:
```cpp
typedef float f16 __attribute__((aligned(16)));
f16 buffer[16];
```

From GCC:
```
error: alignment of array elements is greater than element size
   11 |     f16 buffer[16];
      |                  ^
```

From Clang:
```
error: size of array element of type 'f16' (aka 'float') (4 bytes) isn't a multiple of its alignment (16 bytes)
    7 | f16 buffer[16];
      |           ^
```

How the compiler can allow pointer arithmetic under such conditions is beyond me.

Don't have aliasing pointers / references, that can create dependencies between elements that would be mapped to the same SIMD register, which breaks auto vectorization [(5)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION0010110000000000000000).

Compile with `-ftree-vectorize -fopt-info-vec-missed -fopt-info-vec` to GCC to make it vectorize more and to learn where auto vectorization failed and why.

The simpler you write the code, the more likely the compiler will be able to optimize it.

# Keep Assembly Code Small

This is good because it make it possible to keep a larger fraction of the program in the instruction cache and lower cache levels, and leaves more room for other data.

One way to reduce code size in auto vectorized code is to make it possible for the compiler to prove that buffers are aligned and that the number of elements in the buffers is a multiple of the SIMD width so that no scalar tail loop need to be generated.
For concrete tips on this see _Help The Auto Vectorizer_ section in this chapter.

I don't think the buffer alignment bit is as important anymore, compiler seem to emit unaligned loads regardless.
Though in [_Data-Oriented Design_ > _Help The Compiler_ > _Auto Vectorization_](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION0010110000000000000000) Richard Fabian is very clear in that fixing up the alignment of pointers can improve performance dramatically.
Not sure how.

Code size effects are difficult to demonstrate in micro benchmarks since they, by definition, are micro in usually fit in cache.


# References

- 1: [_What Every Programmer Should Know About Memory_ by Ulrich Drepper @ freebsd.org 2007](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
- 2: [_Andrew Kelley: A Practical Guide to Applying Data Oriented Design (DoD)_ by Andrew Kelley, ChimiChanga @ youtube.com 2024](https://www.youtube.com/watch?v=IroPQ150F6c)
- 3: [_Handles are the better pointers_ by Andre Weissflog @ flooh.gitbug.io 2018](https://floooh.github.io/2018/06/17/handles-vs-pointers.html)
- 4: [_Data-Oriented Design_ > _Existential Processing_ by Richard Fabian, 2018]([https://www.dataorienteddesign.com/dodbook](https://www.dataorienteddesign.com/dodbook/node4.html))
- 5: [_Data-Oriented Design_ > _Helping the Compiler_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/node10.html)
- 6: [_Why is processing a sorted array faster than processing an unsorted array?_ by GManNickG, Mysticial @ stackoverflow.com 2012](https://stackoverflow.com/questions/11227809/why-is-processing-a-sorted-array-faster-than-processing-an-unsorted-array)
