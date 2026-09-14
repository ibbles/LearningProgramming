
# Benefits

For one particular team switching to agentic coding lead to a speed-up of 2x-3x[(1)](https://youtu.be/o08YDtDln7k?t=120).
This makes it possible to build things and try new things that would not have been possible otherwise [(1)](https://youtu.be/o08YDtDln7k?t=154).
Speed encourages experimentation which leads to rapid feedback[(1)](https://youtu.be/o08YDtDln7k?t=162).

By having access to easily accessible knowledge means that we can solve problems outside of our area of expertise without having to involve the domain experts [(1)](https://youtu.be/o08YDtDln7k?t=179).
Meaning that we can get a solution faster and the domain expert can continue working on whatever they are working on without interruption.
Meaning that we increase the risk of breaking things we don't really understand as soon as our system deviates from a "default system".


# Challenges

Things tend to go horribly wrong when using AI coding agents [(1)](https://youtu.be/o08YDtDln7k?t=253).
One study found an increase of up to 41% more defects without any increase in throughput [(1)](https://youtu.be/o08YDtDln7k?t=268).
Initial gains in developer speed was canceled out by increasing code complexity after two months [(1)](https://youtu.be/o08YDtDln7k?t=284).

AI operates in self-harm mode, writing code it cannot reliably maintain later [(1)](https://youtu.be/o08YDtDln7k?t=355).
It becomes a self-enforcing death-march loop [(1)](https://youtu.be/o08YDtDln7k?t=950).
Agentic coders are very good at making prototypes [(1)](https://youtu.be/o08YDtDln7k?t=1006), not so good at maintaining released software where design, code quality, and architecture really matter [(1)](https://youtu.be/o08YDtDln7k?t=1038).
The consequences of past decisions accumulate [(1)](https://youtu.be/o08YDtDln7k?t=1108).


# Code Health

Factors that can be measured from the code [(1)](https://youtu.be/o08YDtDln7k?t=395).
Code with poor code health is unfriendly both to humans and coding agents [(1)](https://youtu.be/o08YDtDln7k?t=453).
Code can be refactored to improve its  health[(1)](https://youtu.be/o08YDtDln7k?t=461)
Working in health code is faster [(1)](https://youtu.be/o08YDtDln7k?t=561) and less error prone [(1)](https://youtu.be/o08YDtDln7k?t=605) for both humans and coding agents.
Making changes to code we don't fully understand is a high-risk business.
Coding agents are even more sensitive than humans to poor code health, with a defect rate that increases faster with deteriorating code health [(1)](https://youtu.be/o08YDtDln7k?t=741).
So we should strive to maintain a very high code health.
Deeply nested logic is responsible for roughly 20% of programmer mistakes [(1)](https://youtu.be/o08YDtDln7k?t=867).
Healthy code burn less tokens per agent task [(1)](https://youtu.be/o08YDtDln7k?t=2078).


# Maintaining Code Health

Have automatic tooling that measure code health and integrate it in the agentic coding loop[(1)](https://youtu.be/o08YDtDln7k?t=1146).
Most existing code have poor code health making it a bottleneck for adoption of agentic coding [(1)](https://youtu.be/o08YDtDln7k?t=1340).
The code health must be improved first, then the agents can be autonomously operating on the code base [(1)](https://youtu.be/o08YDtDln7k?t=1349).
This work can be agent supported.

Automate safeguards with MCP servers, linting tools, code coverage, etc [(1)](https://youtu.be/o08YDtDln7k?t=1841).


# Maintaining Correctness

Test automation is very important, both unit tests and end-to-end tests, otherwise verification and validation will become a bottleneck[(1)](https://youtu.be/o08YDtDln7k?t=1628).
New and modified code should have 100% code coverage [(1)](https://youtu.be/o08YDtDln7k?t=1654).
Existing code should also have 100% code coverage because coding agents have a tendency to fix failing tests by deleting them [(1)](https://youtu.be/o08YDtDln7k?t=1671).
Instead of lowering the barrier to 98% to avoid having to design tests for tricky to test code, prefer to add manual exceptions for those precise places [(1)](https://youtu.be/o08YDtDln7k?t=1720).
That makes the exceptions explicit instead of accidents [(1)](https://youtu.be/o08YDtDln7k?t=1750).

We as human can no longer understand every line of the code base [(1)](https://youtu.be/o08YDtDln7k?t=1766).
Focus human code review on tests [(1)](https://youtu.be/o08YDtDln7k?t=1801).
After leaving plan mode with the agent, have it generate end-to-end tests first and spend some time reviewing and iterating on those together with the agent [(1)](https://youtu.be/o08YDtDln7k?t=1817).


# Design And Architecture

AI-friendliness must be scaled to the design and architectural level [(1)](https://youtu.be/o08YDtDln7k?t=1906).
We currently do now have guardrails and tools in place for this kind of work.
These tools must be flexible enough to be applicable to any type of software [(1)](https://youtu.be/o08YDtDln7k?t=1920).
Principles more than hard rules [(1)](https://youtu.be/o08YDtDln7k?t=1932).

CLEAR: a set of optimization targets for software systems that are designed to evolve safely with agents [(1)](https://youtu.be/o08YDtDln7k?t=1946):
- C: Conceptual alignment
- L: Local reasoning
- E: Explicit intent
- A: Avoid search luck
- R: Reduce the edit surface

Goal: limit the blast radius during software evolution [(1)](https://youtu.be/o08YDtDln7k?t=1984).

# References

- 1: [_Agentic AI Coding Practices for Speed with Quality_ by Adam Tornhill @ youtube.com 2026](https://www.youtube.com/watch?v=o08YDtDln7k)
