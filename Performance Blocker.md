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
If the compiler cannot prove that two pointers cannot alias then the value of one cannot be cached in a register across a write to the other and must be read from memory again.
This is true even if the programmer knows that the two pointers can never alias.
This is true not only for pairs of pointers / references but also a pointer / reference and a member variable [(2)](https://www.youtube.com/watch?v=PPJtJzT2U04).
There is an example of this below.

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

An example of a pointer + member variable aliasing [(2)](https://www.youtube.com/watch?v=PPJtJzT2U04):
```cpp
#include <span>

template<typename AccumulatorT>
struct Accumulator
{
	AccumulatorT accumulator;

	template<typename ElementT>
	void accumulate(const std::span<ElementT>& elements)
	{
		for (const ElementT& e : elements)
		{
			// e may alias with this->accumulator.
			accumulator += e;
		}
	}
};

template class Accumulator<int>;
template void Accumulator<int>::accumulate<int>(const std::span<int>&);

int main()
{
	Accumulator<int> accumulator;
	accumulator.accumulate(std::span(&accumulator.accumulator, 1));
}
```

The main loop of `accumulate` compiles, by GCC 15.2 `-O2`, to
```asm
mov edx, DWORD PTR [rdi]              ; Load accumulator from memory.
.L3:
        add     edx, DWORD PTR [rax]  ; Load e from memory, add to accumulator.
        add     rax, 4                ; Step to next e.
        mov     DWORD PTR [rdi], edx  ; Write accumulator to memory.
        cmp     rax, rcx              ; Check if we are done.
        jne     .L3                   ; Jump back up if not done.
```

The compiler must emit memory load and store instructions in the loop because `AccumulatorT` and `ElementT` are the same type in this case and can therefore alias each other, which is precisely what is done in `main`.
If we know that `elements` will never alias the `accumulator` member variable then we can tell that to the compiler by introducing a new variable that the compiler can see the entire lifetime of when compiling `accumulate`, i.e. a local variable.

```cpp
template<typename ElementT>
void accumulate(const std::span<ElementT>& elements)
{
    AccumulatorT a {}; // Local variable.
    for (const ElementT& e : elements)
    {
        // Aliasing no longer possible since a didn't exist when elements was created.
        a += e;
    }
    // Update the member accumulator.
    accumulator += a;
}
```

With this tweak we not only save two memory accesses per iteration, also also unlock a whole bunch of other compiler optimizations such as loop unrolling and auto-vectorization.

The strict aliasing rule says that two compatible pointers may not alias.
Or rather that we may not access a value through a pointer of an incompatible type.
For example, `int` and `float` are not compatible even though both are four bytes.
The following is not allowed.
```cpp
int getBits(float f)
{
	float* float_ptr = &f;
	int* int_ptr = (int*)float_ptr;
	int bits = *int_ptr; // Illegal, accessing a float value as an int.
	return bits;
}
```

An exception is made for all three `char` types and `std::byte`.
Pointers of these types may be used to access any data, regardless of type.
This is why we can use `memcpy` to implement `getBits`, `memcpy` is allowed to read and write any type.
```cpp
int getBits(float f)
{
	static_assert(sizeif(float) == sizeof(int));
	int bits;
	memcpy(&bits, &f, sizeof(bits)); // OK because memcpy operates on individual bytes.
	return bits;
}
```

The opposite direction, having a `std::byte` buffer but using it as another type, is not allowed.
```cpp
void message_callback(Socket& socket)
{
	std::byte buffer[MESSAGE_SIZE];
	socket.receive(&buffer, MESSAGE_SIZE);
	Message* message = (Message*)buffer;
	// Dereferencing message would be undefined behavior since
	// message actually poitns to a std::byte array, not a
	// Message.
}
```

We must be aware of the aliasing requirements of functions we call.
For example, `memcpy` does not allow the two memory areas to overlap, but `memmove` does allow them to.
In functions you write, be clear about what aliasing assumptions it makes.

Inlining can make it possible for the compiler to prove that at a particular call site there is no aliasing between two pointer / reference parameters and thus that particular instantiation of the function can be more aggressively optimized than the non-inlined instantiation.


# Branches In Loops

Compilers and CPUs really like data-parallel operations.
These often show up in loops when processing arrays.
In good circumstances the compiler can unroll such loops and use vector instructions in the loop body.
This may be prevented when there are branches in the loop body, especially when one branch has a `return`.

For example this loops that checks for ASCII-strings  [(4)](https://lemire.me/blog/2025/12/20/performance-trick-optimistic-vs-pessimistic-checks/):
```cpp
bool is_ascii_pessimistic(const char *data, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (static_cast<unsigned char>(data[i]) > 0x7F) {
      return false;
    }
  }
  return true;
}
```

GCC 16.1 produces the following for the main loop when compiling with `-O2`:
```asm
.L10:
        add     rdi, 1    ; Move to next character.
        cmp     rdi, rax  ; Check if end of string.
        je      .L5       ; Return if end of string.
        cmp     BYTE PTR [rdi], 0  ; Read the byte, compare with zero.
        jns     .L10      ; Jump if not sign, i.e. continue if the 0x80 bit is cleared.
```
This code is reading and processing one byte at a time, which is slow.

If we expect that most strings are ASCII then we can tune for the case where the early out doesn't happen [(4)](https://lemire.me/blog/2025/12/20/performance-trick-optimistic-vs-pessimistic-checks/).
That is, we can optimize for the case where all bytes will be touched.
We do that by not returning early in the loop, but always doing the same work on all elements.
That will produce a loop body that is easier for the compiler to convert to vector instructions.
Exactly how to do that will vary depending on what the loop does.
In this case we can note that we want to detect if any byte has the 8th bit set, which we can do by or:ing all the bytes.
```cpp
bool is_ascii_optimistic(const char* data, size_t length) {
  unsigned char result = 0;
  for (size_t i = 0; i < length; i++) {
    result |= static_cast<unsigned char>(data[i]);
  }
  return result <= 0x7F;
}
```

This form produces a lot more assembly code when compiled with GCC 16.1 and `-O2`.
However, the bulk of the loop is quite small:
```asm
.L14:
        movdqu  xmm2, XMMWORD PTR [rax]  ; Move 16 bytes from memory.
        add     rax, 16     ; Increment index.
        por     xmm0, xmm2  ; result |= on 16 bytes.
        cmp     rcx, rax    ; Check if this was the last full 16-byte block.
        jne     .L14        ; Loop again if not the last.
```
Here we process 16 bytes at the time and after this loop a reduction is performed to take the 16 partial results and combine them into a single result, and then a scalar tail-loop if the number of elements in the array isn't an even number of 16.

We can better understand why the if-return loop cannot be vectorized by the compiler by considering memory accesses [(4)](https://lemire.me/blog/2025/12/20/performance-trick-optimistic-vs-pessimistic-checks/#comment-676386).
A program may never read outside of allocated memory, accessing unmapped memory will lead to segmentation fault, which is a hardware signaled error.
The if-return loop will not access any byte following a non-ASCII character and the compiler is not allowed to transform the code in a way that would add an access to a memory address that a naive machine code translation of the original code would not access.
(
I'm not sure this is entirely true, I don't think the rules are quite that strict.
It may be that the compiler is only forbidden from adding memory accesses that would touch an otherwise untouched page, not address.
Is it always safe / allowed to access any byte within a page as long as at least one byte within that page is known to be accessible?
Clang's documentation for `__builtin_assume_dereferenceable` talks about trapping [(5)](https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume-dereferenceable).

> __builtin_assume_dereferenceable guaranteeing 'size' bytes can be loaded speculatively without trapping.

Is a page fault a type of trap?
)
It is very difficult to vectorize such code since we cannot safely load a block of bytes unless we know that the only place in that block that there can be a non-ASCII byte is the very last byte, something we cannot know until we have read the bytes.
We as humans may assume that when a function is given a (pointer, size) pair then it should be safe to access `size` elements from `pointer`, but this may not always be true is not something that a compiler can rely on.
There are platform-specific ways to tell the compiler that a range of addresses are safe to access.
Clang has `__builtin_assume_dereferenceable(pointer, size)` [(5)](https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume-dereferenceable).
Adding `__builtin_assume_dereferenceable(data, length);` to the top of the if-return loop will let Clang vectorize the loop [(4)](https://lemire.me/blog/2025/12/20/performance-trick-optimistic-vs-pessimistic-checks/#comment-676386).

```asm
.LBB0_4:
        movdqu   (%rdi,%r8), %xmm0  ; Load 16 bytes from memory.
        pmovmskb %xmm0, %r9d   ; Extract the most significant bit from each byte.
        testl    %r9d, %r9d    ; Logical AND of the most significant bits.
        jne      .LBB0_6       ; Return if any bit was set.
        cmpq     %r8, %rdx     ; Check if we are done.
        leaq     16(%r8), %r8  ; Compute starting address of next 16 bytes.
        jne      .LBB0_4       ; Start next loop iteration.
```



# Stalled Cycles

A stalled cycle is one where the CPU can make no progress, i.e. not execute any instruction, because it is waiting for something [(1)](https://www.dataorienteddesign.com/dodbook/node10.html#SECTION001090000000000000000).


# References

- 1: [_Data-Oriented Design_ > _Helping the compiler_ by Richard Fabian @ dataorienteddesign.com, 2018](https://www.dataorienteddesign.com/dodbook/node10.html)
- 2: [_[AoCO 15/25] Surprising Aliasing _ by Matt Godbolt @ youtube.com, 2025](https://www.youtube.com/watch?v=PPJtJzT2U04)
- 3: [_The joys and perils of C and C++ aliasing, Part 1_ by Martin Sebor @ redhat.com, 2020](https://developers.redhat.com/blog/2020/06/02/the-joys-and-perils-of-c-and-c-aliasing-part-1#)
- 4: [_Performance trick : optimistic vs pessimistic checks_ by Daniel Lemire @ lemire.me 2025](https://lemire.me/blog/2025/12/20/performance-trick-optimistic-vs-pessimistic-checks/)
- 5: [_`__builtin_assume_dereferenceable` @ clang.llvm.org](https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume-dereferenceable)

