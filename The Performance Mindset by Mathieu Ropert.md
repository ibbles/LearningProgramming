This is a recording of a presentation held at SwedenCpp in 2026.

Not a talk about coding and optimization techniques [(1)](https://youtu.be/o-C6puc7nOk?t=615).
See Mathieu Ropert's other talks for that:
- [The Basics of Profiling - Mathieu Ropert - ACCU 2022](https://www.youtube.com/watch?v=vqeXRFW26kg)
- [Multi Threading Model in Paradox Games: Past, Present and Future - Mathieu Ropert - ACCU 2023](https://www.youtube.com/watch?v=M6rTceqNiNg)
- [Data Oriented Design and Entity Component System Explained - Mathieu Ropert - ACCU 2024](https://www.youtube.com/watch?v=xm4AQj5PHT4)
- [Heaps Don’t Lie - Guidelines for Memory Allocation in C++ - Mathieu Ropert - ACCU 2025](https://www.youtube.com/watch?v=74WOvgGsyxs)

This is a talk about why the program wasn't fast [(1)](https://youtu.be/o-C6puc7nOk?t=643).

Theory 1: Every project needs a genius [(1)](https://youtu.be/o-C6puc7nOk?t=721).

Theory 2: The program is slow because it doesn't actually seem slow. It is fast enough for the current use-cases [(1)](https://youtu.be/o-C6puc7nOk?t=741).
How would we notice that a program is slow [(1)](https://youtu.be/o-C6puc7nOk?t=751)?
How do we know that the program is not fast enough?
- Users will tell you.
- Telemetry [(1)](https://youtu.be/o-C6puc7nOk?t=828).
- CI benchmarks [(1)](https://youtu.be/o-C6puc7nOk?t=840).
- Dashboards /performance trend [(1)](https://youtu.be/o-C6puc7nOk?t=859).
- Use your own product [(1)](https://youtu.be/o-C6puc7nOk?t=871).

Run a profiler [(1)](https://youtu.be/o-C6puc7nOk?t=975).
Often.
Make sure you have a profile easy accessible, always [(1)](https://youtu.be/o-C6puc7nOk?t=1016).
Don't do work to produce a result you immediately throw away most of [(1)](https://youtu.be/o-C6puc7nOk?t=1052).

Intel Threading Building Blocks [(1)](https://youtu.be/o-C6puc7nOk?t=1227).
Good thread pool, work stealing scheduler.

Profiling [(1)](https://youtu.be/o-C6puc7nOk?t=1371).
Can find unnecessary copies.
Can find performance regressions.
Profile more.

Theory 3: No one thinks it's an issue [(1)](https://youtu.be/o-C6puc7nOk?t=1426).
Do we have a frame of reference [(1)](https://youtu.be/o-C6puc7nOk?t=1455)?
How do we know that "fast" and "slow" means?
Are we all in agreement?
Are the developers in agreement with the users?
Good if you can have something that sets a time budget.
In games that is the target framerate [(1)](https://youtu.be/o-C6puc7nOk?t=1495).
8 ms for 120 FPS, 16 ms for 60 fps, 33 ms for 30 fps.
Difficult to set a time budget without access to the final hardware that will run the software.
Set hardware requirements?

You should be aware of how long different operations take [(1)](https://youtu.be/o-C6puc7nOk?t=1550).
To be able to tell when something isn't right and could be better [(1)](https://youtu.be/o-C6puc7nOk?t=1627).

Theory 4: Too hard to fix [(1)](https://youtu.be/o-C6puc7nOk?t=1687)?
One fix is to get better, learn new things and tricks.
After a while you will become familiar with the most common causes of performance problems.

Theory 5: Can't find the time [(1)](https://youtu.be/o-C6puc7nOk?t=2007).
You cannot always expect to be given time dedicated to look at profiler output and optimization opportunities.
It should be baked into the regular work, or you need to take time yourself.

Common excuses [(1)](https://youtu.be/o-C6puc7nOk?t=2239):
- There are other priorities.
- Schedule leaves no time for performance.
- Made a ticket / issue.
	- That no one will look at.
- My boss won't let me [(1)](https://youtu.be/o-C6puc7nOk?t=2416).

Look for quick wins [(1)](https://youtu.be/o-C6puc7nOk?t=2582).
Use the result to look for more time.


The performance mindset [(1)](https://youtu.be/o-C6puc7nOk?t=2684):
- Know that performance can be improved.
	- Don't give up.
- Run a profiler. Often.
- Learn new things. Look for inspiration and patterns.
	- Talk to your colleges about what you are doing and what they are doing or have done.
- Fix first, ask permissions later.

Questions / comments:
Put work on a background thread.
Do the work asynchronously.
Delay work until later when there is less pressure.
Parallelize loops.
Do work on the GPU.
Don't do work that doesn't need to be done.
Keep track of the critical path.
Don't put too much hope into microbenchmarks and microarchitecture optimizations [(1)](https://youtu.be/o-C6puc7nOk?t=3098).
Beware that any code you add comes with a maintenance cost for the team [(1)](https://youtu.be/o-C6puc7nOk?t=3278).


# References

- 1: [_Mathieu Ropert: The Performance Mindset_ by Mathiue Ropert, SwedenCpp @ youtube.com 2026](https://www.youtube.com/watch?v=o-C6puc7nOk)
