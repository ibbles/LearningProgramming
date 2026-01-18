Data-oriented design is the act of designing and developing software in a way that makes the data, data flow, and operations performed on the data, first-class citizens.

Data-oriented design is the result of the observation that the computational performance characteristics of computers changed a lot in the 2005 - 2010 time-frame along with a desire to keep improving the runtime performance of our programs.
The two main changes to computers in this era is the increasing divergence in speed between arithmetic operations and memory, along with an increase in available parallelism both in terms of the number of cores, the number of in-flight instructions per core, and the number of data-elements processed per instructions.
All of these factors influence how high-performance software should be written.

The two main building blocks in a data-oriented design is data and transformations performed on that data.
Data-oriented design is not just about reducing cache-misses.
Data-oriented design is not just about the schema for how data is stored in memory [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=4713).
Data-oriented design is not just about how data is transformed.

A core tenet of data-oriented design is to not hide the data behind abstractions, and instead operate directly on it.
Our knowledge of the data and the hardware our program is running on allows us to make decisions about how the data should be operated on by the program.
Avoid entangling data in dependencies and object lifetimes that are not inherent to the data but accidental complexity cause by the way we store data together with other, seemingly related, data.
A data element's location in memory and lifetime is just as important as it's type and value.
Avoid creating monolithic classes that entangle data, where data is stored, and what it's lifetime is across multiple pieces of data for which these things may not actually be the same.

Avoid using frameworks that have been designed with generality in mind, at the expense of the option to craft custom solutions for custom situations.

Data-oriented design means that we are designing our architectures with a major focus on data layout, not that we apply a prescribed data layout to our data [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=4713).
Let the access patterns choose the layout, not the other way around.
The access patterns are determined by the problem being solved and the solution, in an abstract sense, employed to solve that problem.
Abstractions over the data storage makes it easier to switch back and forth between different storage layouts, which is required for efficient experimentation.

Data is not the problem domain.
A common technique in software development is to bring the problem domain into the types of the program.
To express the mental model, the description of the domain, directly in the program source code and to structure the design of the program around those concepts.
Instead, in the object-oriented approach, the data is kept simple and computationally efficient, while the problem domain is expressed in the data transformations.
The goal is to not let high-level concepts of the problem domain restrict the implementation of low-level operations.

In data-oriented design we consider the data we actually have and the operations we know we need to perform.
A particular problem is solved as directly as possible, rather than planning for all eventualities.
It is better for a piece of code to be replaceable than to be extendable, this means it can be simpler.
Holding on to the past hinders flexibility.
Future-proofing is difficult since we cannot see the future.
By not tying the software architecture to the business facts we make the software easier to change when the business facts inevitable change.
We avoid coupling the pieces making up the implementation to the problem domain.
Data-oriented design handles change well since data and transformations can be coupled and decoupled at will.

Operations should generally no be associated directly with individual elements of bulk data.
Instead the bulk data should be considered as a whole in each transformation.
When we as programmers reason about collections of simple data types we are more likely to think in terms of well-known data structures and algorithms, compared to when we are forced to reason about high-level concepts that are specific to a particular problem domain.
The higher level of abstraction prevents us from seeing the details that would provide clues to a better data representation of a different algorithmic approach.

Data structures should not live inside objects, object data should live inside data structures.
This simplifies data manipulation

There is more to data than its structure.
(Such as?)

Move away from a web of connected complex objects to a simpler model of arrays.
At least when operating at a fine granularity, i.e. when there are many similar objects [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=4818).
Object-oriented programming works well to express at a coarser granularity.
For example, have an OOP `ParticleSystem` that contains internal DOD particles, maybe in a SOA storage format.
Use OOP at the API layer, use DOD at the data storage and processing layer.
OOP is the interface, DOD is the implementation.
OOP is the shell, DOD is the engine.


# The Data-Oriented Mindset

Data-oriented design is not an implementation instruction but a mindset [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=1916), [(3](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=4713).
- Model a world of data transformation.
	- Code is a pipeline that transforms data from one state to another.
	- We don't (really) even care about which data belong together in an object, for each transformation we only care about the data needed to perform that specific transformation.
- Operate directly on batches of data.
	- Instead of individual objects being in control, it is now loops that work in bulk.
	- "Managers" or "systems" are responsible for behavior.
	- Processors thrive on simple predictable work, which means long straight runs on continuous data.
- Expose data, centralize behavior.
	- Data is the most important thing, don't hide it.
	- Behavior implementations should see all the data so that it can process it in the best way possible.
	- Make sure you understand its shape, size, and access patterns.
	- Design for the hardware, not a metaphor of the problem.
	- This means that you should be aware of the capabilities and limitations of the hardware.
- Plat for today.
	- Design for the problem you actually have.
	- Don't add complexity to solve that problem that doesn't exist.
	- Once the problem do change, which it inevitable will, by not adding an abstraction that went in the wrong direction we made it easier to adapt to that change.

Focus on the data's journey.


# The Folly Of Generic Solutions

Data-oriented design is work-flow specific.
Data-oriented design is a process, not a data structure or a library [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=4713).
Data usage patterns are not generic.
While a data-parallel for-loop over a linear array is a fundamental building-block, in a particular part of a particular software there may be better, more specific, approaches than a data-parallel for-loop.
For example, if we have application-specific knowledge about our data then we can chose, or even design, a custom sorting algorithm that performs better than a generic library-provided sort implementation, or we can bake additional work directly into the sorting process.


# Properties Of Data

Things to be aware of:
- What is tying your data together? Is it a concept or implied meaning?
- Is your data layout defined by a single interpretation from a single point of view?
- Think about how the data could be reinterpreted and cut along those lines.
- What is it about the data that makes it uniquely important?
- What streams of data exists?
- Who or what consumes each stream of data?
- What are the access patterns to the data?
- For each memory transfer, how much of the fetched data is used?
- For each memory transfer, how many compute operations are performed per element?
	- This describe the compute / bandwidth requirements of the application.
- For each memory transfer, how much of the fetch data is modified?
- How often is each piece of data read?
- Is there any implicit information in the data, that is not stored explicitly?

All data has some level of complexity.
A single array has a low degree of complexity.
Multiple parallel arrays has some additional complexity, but not much.
Arrays that contain references to elements in other arrays has a higher complexity.
Arrays that contains sub-arrays increases complexity further.
Sparse arrays increases the complexity further still.

Data can reference other data.
This is often done by having one piece of data containing a pointer or reference to some other piece of data.
A reference can also take the form of an index into an array.
A reference can also take the form of an ID that is used to look up the location of the referenced data.
The referenced data must exist before we can create a reference to it.
Otherwise it doesn't yet have an address to point to, an index in an array, or an ID in a look-up table.
This can be problematic if we have circular reference chains, or if we need a reference to an object that will be created in the future.
One way around this is to separate the identity of each object from the data that describe the object.
We can say beforehand that a particular object will exist in the future without being ready to actually create it.
The mere declaration that an object will exist at some point in the future is sufficient to give it an ID and thus a way to reference it.


# Types Of Operations

Most operations performed by a functions is one of:
- insert
- update
- delete
- query

By understanding the characteristics of our data and our operations we can chose implementations of both data storage and algorithms that are efficient in some way that we care about.

By keeping the data storage simple and the cleverness in the operations we do not need to maintain a deep understanding of the data format to use it well.


# Principles

There are the principles from [_Data-Oriented Design_ by Richard Fabian](https://www.dataorienteddesign.com/dodbook/dodmain.html).

- 1: Data is not the problem domain.
- 2: Data is the type, frequency, quantity, shape, and probability.


# Object References

There are multiple ways to reference another object [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=2218).

- Pointer .
	- The referencer has a pointer to the referencee.
	- This works well in the classical object-oriented, single-allocation paradigm where objects are allocated individually on the heap and stay there until the end of their lifetime.
	- Require address stability of the referencee, i.e. it may not move.
	- Don't use pointers to objects stored in an `std::vector` unless you can guarantee that the container won't be reallocated or reordered.
	- Don't use pointers to objects stored in an array unless you can guarantee that the array won't be moved along with the outer objects, and that the array won't be reordered.
- Index.
	- Used when many objects of the same type are allocated together in a container.
	- In this case we (typically) cannot use a pointer because the container may be reallocated.
	- Don't use indices to objects stored in a container that can be reordered.
	- A technique to maintain index stability even when other elements in the same container are destroyed is to leave a tombstone at the index of the destroyed element. The tombstone can be a member on the element (Such as an `is_alive` flag.), a sentinel byte pattern ( Such as `memset(0)`.), a sentinel value (Such as a default-constructed object.), or a wrapper type to signal absence / presence of a value (Such as `std::optional`.). When creating a new element re-use a tombstone index if available rather than resizing the container, for example using a free list.
- ID.
	- An ID identifies an object based on its identity rather than its location.
	- To get at the data we need to find the location, which often involves some kind of look-up.


# Effective Cache Utilization

A possible goal when designing with data in mind is to make effective use of the CPU's cache.
An access is fast if it can be serviced from the cache.
An access is slow if it must be serviced from main memory.
Memory is accessed in chunks called cache lines.
A cache line is often 64 B on a desktop machine.
A guess for the cache line size can be had a compile time with [`std::hardware_constructive_interference_size` and `std::hardware_destructive_interference_size`](https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size.html) in C++.

In short, as many memory accesses as possible should be serviced from the cache for best performance.
One way cache space is wasted is when pieces of data that are not used together are stored close to each other in memory.
When the first piece is accessed the other piece will get pulled in as well if they lie on the same cache line, which is wasted cache space and memory bandwidth since we are only going to use the first piece.
It would be better if the entire cache line contained useful data.
By structuring our data carefully in memory we can minimize the amount of wasted cache line space.

One strategy for data organization intended for effective cache utilization is to store each similar piece of data for many objects together, and to do batch operations on all objects at once.
For example, by storing the positions of a bunch of particles in an array when we go update those positions and read the first particle's position we will populate the cache with the neighboring particles positions' as well.
Even better, since the update will walk linearly through the positions array the hardware memory prefetcher will quickly discover the pattern and start pulling in upcoming particle positions for us ahead of time.
For more on this, see the _structure-of-arrays_ chapter.


# Types Of Data Storage

This chapter summarizes a few ways of storing data.
Some of these have dedicated chapters or sections.

- Array-of-structs.
- Struct-of-arrays.
- Three-of-objects.
- Network-of-objects.


# Types Of Data Processing

This chapter summarizes a few ways of processing data.
Some of these have dedicated chapters or sections.

- Global storage.
	- Each phase of the computation reads from and writes to a global data store.
- Stream / pipeline.
	- Data is generated and transformed through a pipeline of transformers / filters / ....
- Events.
	- Events trigger the generation of data, which may trigger other events for processing of that data.


# Implementation Suggestions

Here are a few things to consider doing when designing and implementing a program [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=2109).

## Avoid Individual Heap Allocations

Instead store each type in its own contiguous container.
Avoid a single large polymorphic container, instead have many smaller container with concrete types.


## Encode Implicit Data In Containers

We can save some memory by implicitly encoding information in which container an entity is stored in [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3610).
For example, instead of storing type information as an `enum` in each entity we can have one container for each type.
For example, we can have a `fireParticles` container and a `smokeParticles` container, both containing `Particle` entities.



## Flatten The Inheritance Hierarchy

## Decouple Data From Logic

## Dirty Sets Instead Of Dirty Flags

It is common to have data whose value depend on the values of other data.
For example, the sum of a container of values depend on the values in the container, and the transformation of an object in a transformation hierarchy depends on the transformations of all parents in that hierarchy.
If the dependent value is expensive to compute we may not want to do it eagerly since the value we spend effort computing may never be used before it is recalculated.
One way to handle this is to have a dirty flag.
Any change that invalidates the dependent value sets the dirty flag and once the dependent value is actually needed we check the dirty flag and recalculate if necessary.
A drawback of this approach is that we pay for a branch and possibly a misprediction every time the value is needed [(1)](https://www.dataorienteddesign.com/dodbook/node9.html#SECTION00970000000000000000).
If reading the flag causes a cache miss then the cost of reading the flag and acting upon it may be greater than the cost of computing the value.
An alternative to a dirty flag is a dirty set [(1)](https://www.dataorienteddesign.com/dodbook/node9.html#SECTION00970000000000000000).
This technique is applicable when there are well-defined points within the application life cycle where we transition from a write-phase where the dependee data may be written to and a read-phase where the dependent data is read from.
Instead of doing dirty tracking as mutable state stored within each and every object we have a container containing the dirty objects.
As we transition from the write-phase to the read-phase we do a single pass over that container and update the dependent data on all dirty objects.
This has a few advantages:
- Batch calculation.
	- When the dependent data is computed we only touch objects that are dirty and should be recalculated.
	- We only do that particular calculation repeatedly, the calculation is not mixed in with random calculations spread out all over our application.
	- This makes better use of the instruction cache.
- Optimal reads.
	- When we need to read the dependent data we know that the value is up-to-date.
	- No need to check any flag, no risk for an extra cache miss when reading it, and no risk of a branch misprediction.
	- No risk of uneven processing times in the read-phase of the application.
- Clearer performance analysis.
	- By doing the value update in a single places it becomes easier to quantify it during profiling.
	- The cost of the update is not scattered all over the read-phase of the application, making it impossible to measure and misconstruing the actual cost of the read-phase of the work.

A drawback, as mentioned above, is that it requires strict checkpoints in the application that separates the write-phases from the read-phases.
We can no longer willy-nilly read and write the dependent data like we can with a developer-friendly dirty flag.


## Structure-Of-Arrays

Structure-of-arrays means that we have a `struct` that contains a bunch of arrays [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3804) that contains a collection of entities of the same type.
This is an alternative to the more common array-of-structures, where we have a single array that contains many instances of a `struct` type.
In Structure-of-arrays each member of all entities of a particular type are stored together in their own memory block.
```cpp
struct Particles
{
	std::vector<Vector3> positions;
	std::vector<Vector3> velocities;
	std::vector<Vector3> accelerations;
	std::vector<float> scale;
	std::vector<float> opacity;
	std::vector<float> rotation;
};
```

There is no longer a memory area that contains all the data for an entity [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3897).
Instead all the data for an entity live at a particular index in these containers.
All containers have the same size and grow and shrink together.

An advantage of structure-of-arrays is that when accessing a subset of the members we don't pollute the cache with the other members, sometimes called surgical field loading [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3975).
Also, we reduce the amount of memory wasted to padding since the compiler won't need to insert any between elements of an array that same way it may need to do between members of a `struct` due to alignment requirements [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3907).
Also, we make it easier to add [[SIMD]] acceleration [(3)](https://youtu.be/SzjJfKHygaQ?list=PLkDceauvDXDyD_7gVFEG6ASTcBTPZaEvH&t=3987).



# References

- 1: [_Data-Oriented Design_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/dodmain.html)
- 2: [_Data-Oriented Programming_ by Yehonathan Sharvit, 2022](https://www.manning.com/books/data-oriented-programming)
- 3: [_More Speed & Simplicity: Practical Data-Oriented Design in C++ - Vittorio Romeo - CppCon 2025_ Vittorio Romeo, CppCon @ youtube.com 2025](https://www.youtube.com/watch?v=SzjJfKHygaQ)
- 4: [_Andrew Kelley: A Practical Guide to Applying Data Oriented Design (DoD)_ by Andrew Kelley, ChimiChanga @ youtube.com 2024](https://www.youtube.com/watch?v=IroPQ150F6c)
