Optimization is the act of modifying a program, or a part of a program, so that it finishes in a shorter amount of time, or uses less of some other resource.

Often the problem formulation is a bit to vague to be useful.
A user of a game may complain "_The framerate goes down when I create a bunch of units!_".
Goes down by how much?
From what to what?
How many units?
What type of units?
What are the units doing?
What is the expected framerate?
What is an acceptable framerate?
What hardware is the user running the application on.

It is a good idea to have a clear goal in mind.
A goal is sometimes called a _requirement_.
It can be a global goal such as
"_No frame/tick/update/query should take more than X ms._"
or a more local goal such as
"_The spatial query acceleration structure must not use more than X MiB of memory._".
A goal / requirement should be measurable and have a clear pass / fail test.
There may be qualifications or extra specifications on the goal / requirement, such as
"_The spatial query acceleration structure must not use more than X MiB of memory when holding 1000 objects._".
Specify what hardware should be used for timing tests.
It is good to use the same hardware throughout the project, since that make it possible to compare measurements over time.

When optimizing it is important to decide up-front which part you are optimizing and what you are optimizing for.
A profiler is a good tool to identify what needs optimizing and where in the code resource usage is high.
A profiler can be a tool separate from the program, or built into the program itself.
The purpose of profiling is to eliminate, or at least reduce, guessing.
Profile-guided optimization is generally more successful than guessing-guided optimization, but don't take this as an instruction to ignore experience.

Optimization is a multi-staged process with plenty of thought and decision making going into each stage.
Make sure to take plenty of notes, both of your reasoning and your decisions.


# Measurement

A measurement is taken by running the program and observing some property of it, often with the help of a profiler.
Different types of profilers are used for different things.
Always be on the look-out for the profiling tools.
Look for new ways to use the tools you already have.
Look for new places to use the tools you already have.
Look for things you current tools cannot find, and then look for tools that can find them.
Make new tools if necessary or ask others to make them for you, you are probably not the only one who would have use for them.
In other words, find your blind spots and eliminate them.
I good understanding of the hardware and platform software is necessary.



## Measurement Quality

It is important to be aware of the accuracy of your measurements and to be taking suitable steps to handle imperfect measurements.
Some knowledge in statistics is useful.
Take your measurement multiple times to get an understanding of its distribution.
You typically cannot measure the runtime of a program once, do some change, measure again, and then conclude that the change made the program X% faster.

The best type of measurements are those that are reproducible, meaning they produce the same result for the same program and the same input every time.
This is rare with timing based measurement, but can be true for measurements such as memory usage, instruction retire count, number of cache misses, or branch mispredictions.


# Analysis 1 - Initial State

We look at the measurement data and draw conclusions from them.
The conclusion may be that we need more or better data.
This may result in new measurement tools being deployed or implemented.

Based on the measurement data, come up with a few strategies and solutions for rectifying the performance problem.
For each solution estimate impact of implementing that solution.
Do a serious estimation, based on what you know about the program, the input, and the hardware.
Not only will this give you a hint of which of the possible solutions is the best way forward, it is also a learning opportunity.
Not only will thinking about your application, your input, and your hardware in a different way from how you think about them during design and implementation lead to new insights, an optimization effort is interesting in that it provides a ground truth result in the end.
If the estimation you made turned out to be incorrect, can you figure out why the result turned out to be what it in fact turned out to be, and can you understand why your estimation was wrong?


# Implementation

Make the changes that will fix the problem.
It is advantageous if you can do a quick and dirty implementation first to verify that the solution works, that it has the intended effect on the measured property, and that it doesn't cause any unforeseen consequences on other parts of the application.
It helps if the application has been designed in a modular and [[Data-Oriented Design|Data-Oriented]] fashion so that the alternative implementation can be implemented, tested, and analyzed without involving the entire application.
Using the original implementation, run up until the point where the code being optimized is supposed to run, dump all relevant data, run the old code, and dump the result.
Then, using the new implementation, run the new code, and compare the new output with the old output.
This only works if the change is localized, changes to foundational data structures and system design is difficult to test in this way.
Do measurements on the quick-and-dirty implementation to ensure they are in line with the estimates done based on the analysis.
Do the new measurements indicate any areas of improvements in the new implementation?

Once satisfied that the solution is correct and solves the original problem it can be implemented properly in the full application.


# Analysis 2 - Verify Solution

Not only does this step verify that the problem has indeed been solved, it also documents the solution and any conclusions made during the process.
Make sure it is written down somewhere.
By writing you will improve your understanding of the situation and force yourself to finish each though.
This makes it possible for others to learn the optimization technique and the steps taken to identify, analyze, solve, and verify the problem and solution.
It also makes it possible to verify or correct any mistakes done in reasoning or analysis, which will stop you from forming false knowledge.


# Steps

- 1: Identify a problem.
	- Can be vague.
- 2: Quantify the problem.
	- A measurable property that we can set a limit for, such as time or memory usage.
- 3: Measure initial state.
- 4: Analyze initial state measurement.
	- Identify the cause of the problem and formulate possible solutions.
	- Provide estimates for the possible solutions.
- 5: Implement solution.
- 6: Verify solution.


# Enabling Compiler Optimizations

The compiler's optimization phase does a number of transformations on the program in order to transform it from a naive representation of the code we wrote to something equivalent that will run faster [(1)](https://www.dataorienteddesign.com/dodbook/node10.html).
There are limitations to what transformations a compiler is allowed to do, it may only make changes that does not change the semantics of the original code, this is called the _as-if_ rule.
It is common that the code we write expresses limitations that we actually don't care about.
These are called performance pessimisations.
Examples of performance pessimisations are iterating through elements of a memory area in the wrong order and using `std::move` to return from a function that could otherwise have used return value optimization.

See [[Performance Enhancer]] and [[Performance Blocker]].

## Reducing Order Dependence

Static single assignment is a form of code representation where all values are read-only.
Compiler typically do this transformation as an intermediate representation of your code.
We can write our code in this form ourselves by avoiding doing assignments as much as possible.
Not sure if this leads to any performance improvements though.


# Performance Heuristics

Measurements using the actual release version with actual real-world inputs on end-user hardware is the only way to accurate gauge the performance of your software.
A number of heuristics are used to estimate the performance.
This is called _static performance modeling_.
Use these with care since the often aren't very good.
Though some may be able to to put a lower bound on the runtime which is sometimes useful, if nothing else as an ideal to strive towards.

- Instruction count: The number of assembly instructions.
	- A proxy for the amount of work done.
	- Not all instructions take the same amount of time, i.e have the same latency. some are MUCH slower than others.
	- Not all instructions have the same throughput.
	- Unrolled loops often result in more instructions, but the lower iteration count (less overhead, larger fraction of "actual work" instructions) may make up for that.
	- Vectorized code often has more instructions, but the parallel processing often make up for that.
	- If there is a high level of instruction level parallelism, i.e. the critical path is short, then a lot of instructions can be executed very quickly thanks to out-of-order execution and multiple execution units in the CPU.
- The number of arithmetic operations.
	- Similar to instruction count.
- The number of bytes read from and/or written to memory.
	- It can be difficult to count the number of memory accesses by reading the code since the effects of caching and prefetching can be unpredictable. A read from L1 is very different from a read from main memory.
	- Sequential vs random access has a big impact on the cost per memory access due to how well they make use of caching and prefetching.
	- Store buffers and write combining changes the numbers of actual writes that results due to the memory stores you see in the code.
	- If the cache line is loaded into cache by multiple cores then writes to it will cause a lot of cache coherency traffic between the CPU cores, which comes with a cost.
- The number of cache misses.
	- If your code makes good use of the prefetcher and has sufficiently high arithmetic intensity then the CPU will be able to completely hide the memory latency and the number of "cache misses" doesn't really matter.
	- I put "cache misses" in quotes above because here I count the prefetcher's work as cache misses even though it didn't result from a memory load. Some might say that this isn't a cache miss at all. It is a cache miss only if an instruction is stalled because of a load.
- The number of branches.
	- CPUs are highly pipelined which means that a branch misprediction comes with a high cost.
	- That's why some people put a lot of effort into writing branchless code.
	- But branches are only really expensive when there is a misprediction, so complex multi-instruction branchless code may be slower than branchy predictable code.
- Arithmetic intensity: The number of arithmetic operations per byte read from and / or written to memory.
	- Can help estimate if the code is memory bandwidth bound.
	- While many programs are memory bound, if your code is already compute bound then increasing the arithmetic intensity will only make it slower, not faster.
	- While many well-written programs are memory bandwidth limited, if your code is memory latency limited (pointer chasing (`p = p->next`) or double-look-up (`a[b[i]]`)) then increasing the arithmetic intensity won't help you since the CPU is stalled waiting for a memory load most of the time anyway. (I don't think this is quite true. It doesn't matter whether the CPU is waiting because of not enough bandwidth or too high latency, increasing the amount of work we can do per read will help in either case.)
- Critical path length.
	- The longer chain of dependent operations.
	- A short critical path relative to the number of instructions implies a high level of instruction level parallelism.
	- The fastest possible way for the CPU to execute the critical path is a lower bound on the execution time for that block of instructions.
	- However, the critical path is not always the limiting factor. There is often a significant amount of work outside of the critical path that combined take more time than the critical path.
- Asymptotic complexity (big-O)
	- While the asymptotic complexity does tell you about the scaling of the algorithm, it doesn't tell you anything about a particular problem size. That is, for a small `N` the asymptotic complexity can be misleading. For many real-world algorithms that `N` is large enough for the simpler algorithm to win in practice.
- "I was once thought that writing code like this makes it fast."
	- 

A few questions to ask when considering one algorithm / implementation over another:
- Does it reduce passes over data?
- Does it reduce synchronization points?
- Does it expose more instruction level parallelism?
- Does it simplify aliasing assumptions?
- Does it make vectorization obvious?

Even seemingly fast instructions may take a long time to execute.
Consider the humble `addl %esi, (%rcx)`, an integer add to a memory location.
Now consider the case where `rcx` is a pointer to a memory mapped file and a page in that file not currently in memory,
on a mounted network drive,
that we are accessing over a WiFi connection,
and the file is on a spinning-platter drive,
that is currently in a power-safe state and needs to spin up before any data can be read.
I don't even want to guess how many clock cycles that `addl` instruction will stall the thread.
# Resources

- 1: [_Data-Oriented Design_ > _Optimizations_ by Richard Fabian, 2018](https://www.dataorienteddesign.com/dodbook/node9.html#SECTION00930000000000000000)
