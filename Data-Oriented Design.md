Data-oriented design is the act of designing and developing software in a way that makes the data, data flow, and operations performed on the data, first-class citizens.

Data-oriented design is the result of the observation that the computational performance characteristics of computers changed a lot in the 2005 - 2010 time-frame along with a desire to keep improving the runtime performance of our programs.
The two main changes to computers in this era is the increasing divergence in speed between arithmetic operations and memory, along with an increase in available parallelism both in terms of the number of cores, the number of in-flight instructions per core, and the number of data-elements processed per instructions.
All of these factors influence how high-performance software should be written.

The two main building blocks in a data-oriented design is data and transformations performed on that data.
Data-oriented design is not just about reducing cache-misses.
Data-oriented design is not just about the schema for how data is stored in memory.
Data-oriented design is not just about how data is transformed.

A core tenet of data-oriented design is to not hide the data behind abstractions, and instead operate directly on it.
Our knowledge of the data and the hardware our program is running on allows us to make decisions about how the data should be operated on by the program.
Avoid entangling data in dependencies and object lifetimes that are not inherent to the data but accidental complexity cause by the way we store data together with other, seemingly related, data.
A data element's location in memory and lifetime is just as important as it's type and value.
Avoid creating monolithic classes that entangle data, where data is stored, and what it's lifetime is across multiple pieces of data for which these things may not actually be the same.

Avoid using frameworks that have been designed with generality in mind, at the expense of the option to craft custom solutions for custom situations.

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

# The Folly Of Generic Solutions

Data-oriented design is work-flow specific.
Data-oriented design is a process, not a data structure or a library.
Data usage patterns are not generic.
While a data-parallel for-loop over a linear array is a fundamental building-block, in a particular part of a particular software there may be better, more specific, approaches than a data-parallel for-loop.
For example, if we have application-specific knowledge about our data then we can chose, or even design, a custom sorting algorithm that performs better than a generic library-provided sort implementation, or we can bake additional work directly into the sorting process.


# Principles

There are the principles from [_Data-Oriented Design_ by Richard Fabian](https://www.dataorienteddesign.com/dodbook/dodmain.html).

- 1: Data is not the problem domain.
- 2: Data is the type, frequency, quantity, shape, and probability.

# References

- 1: [_Data-Oriented Design_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/dodmain.html)
- 2: [_Data-Oriented Programming_ by Yehonathan Sharvit, 2022](https://www.manning.com/books/data-oriented-programming)
