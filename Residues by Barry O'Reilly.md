# Architecture

Architecture is the structure of a software application.
Architecture is a collection of residues, a series of deeply uncertain architectures (residues) over time.
Architecture drives behavior in an environment.
Architecture determines how a system responds to scale, load, changes, integrations, and non-functional concerns.
Architecture supports functionality, especially changing functionality.
Architecture is software engineering one level above data structures and algorithms.
Architecture work happen both explicitly and implicitly.

Architecture is not a static diagram.
As a first approximation, architecture is a picture that captures a set of components and their relations.
This view of architecture cannot capture time, uncertainty, and change.
The context within which a software operates is ever-changing.
The environment a software applications operates in changes with customers, markets, economies, societies, competitors, and social structures.
These changes cannot be predicted when the initial architecture is designed.
The architecture should be able to keep up with these changes.
The postmortem of a failed software system should never blame poor or incomplete requirements because the environment is (often) inherently uncertain.
We cannot know enough about complex domains to create rigid, lasting structures through the analysis of language or prediction of the future.

Architecture is difficult because software require certainty to be written, but architecture can never rely on certainty.
Random simulation of the environment generates better architectures.

Architecture describe the building blocks of software:
- function
- procedure
- module
- object
- service
- micro-service
- residue

An architecture can be expressed as changing residues over time.
A design criteria is to produce good residues.

# Architect

An architect is someone who has influence over the architecture of a system.
The architect is responsible for ensuring that the architecture can support the system operating in an ever-changing context.
It is difficult to predict in what ways the context will change.
The architecture must be able to survive changes to the context that the architect did not predict.

Limitations
- Cannot describe the entirety of a complex, hyperliminal system.
- Cannot predict what will happen or when in the hyperliminal system.

The goal of the architect is criticality, not correctness.
Correctness is the goal of the programmer.

The architect should answer two questions:
- Am I building the right thing?
- Am I building the thing right?

# Architecture Process

Start with a naive architecture.
The simplest you can think, or first you come to think of, of that will solve the problem.
For example, put all the data in a single container.

Do not try to predict what the attractors of the system will be.
The residuality process will help you find them.

## Residuality Theory In The Architecture Process

Applying residuality theory to the architecture process consists of two steps:

- Stressor Analysis
	- Random simulation of the business environment.
	- Think beyond the current picture.
	- Discover weakness and fault lines in the architecture.
	- Captures uncertainty and change over time.
	- Identify residues.
- Contagion Analysis
	- Uses incidence matrices.
	- Investigate how stress spreads through the system.
	- Identifies coupling.
	- Coupling is caused by integration of residues.


## Business Environment Simulation

A business environment simulation starts with coming up with stressors.
Imaging anything that may happen which have not been considered yet.
When a stressor has been identified, reason about what the associated attractor looks like.
Then ask if the current architecture will survive in this new attractor, i.e. in the system state that the stressor pushed the system into.
If the answer is no, then the architecture must be changed to survive in the new attractor.
After the change the system will consist of two parts:
- Pieces from the old architecture.
	- This is called residue.
- New pieces needed to construct the new architecture.

A business environment simulation is performed multiple times over time with many different stakeholders.
They start from the same original architecture.
Each simulation results in a new residue.

After a number of simulations it will be difficult to come up with stressors that produces attractors that the identified residues doesn't already handle.
This means that the architecture is approaching criticality.
Criticality means that it can survive stressors that the architecture wasn't deliberately design to handle.

After repeated application we have the original architecture and a collection of residues.
Each residue is a description of the changes necessary to the original architecture to produce a new architecture that will survive one stressor / attractor pair.
All residues combined is a description of the architectural changes necessary for the system to survive all stressor / attractor pairs.


## Residual Architecture Is Not Software Architecture

Not all residues need be applied to the architecture, and those that are applied need not be applied immediately.
The integration of all residues into a single architecture produces what is called the residual architecture.
We should not build the software system based on the residual architecture.
That could be prohibitively expensive or difficult.
And with limited utility since the residual architecture contains residues for stressors that may never manifest in the non-ergodic context.
Instead we build the software architecture from a collection of the identified residues based on our best judgement.
The software architecture should be design so that it is possible for the software to move between residues easily.
That makes it possible to quickly react to predicted changes / stressors in the non-ergodic context since the residue that handles that stressor has already been identified.
Contagion analysis helps with this.


# Architectural Walking

Deleuzian Walk, in the book _Difference And Repetition_.



# Functionality

Functionality is what the software does.


# Properties Of A System

## Ergodicity

In this context, ergodicity means that the future of a system is determined by the past.
In other words, if I observe a system over time eventually I will be able to predict future events in that system.
An ergodic system is typically ordered.

Business systems are often not ergodic.
We cannot know what the business system will look like in the future since we cannot predict the environment in which the system will operate in in the future.

Three words that mean almost the same thing in this context:
- complicated
- ergodic
- ordered

Three words that mean the opposite of the three above:
- complex
- non-ergodic
- disordered



## Hyperliminality

A hyperliminal system is a system where a complicated, ergodic, ordered core executes inside a complex, non-ergodic, disordered context.
The outer context is ever-changing, the inner core is slow to respond to those changes.
The design of the architecture influence how fast the core can react to changes.
If the core cannot react fast enough then it will fail.
This can be interpreted as the architecture failing.
The architecture should design an architecture that can survive the changes of the complex context.
This is difficult since the architect cannot know in advance how the complex context will change in the future.
Do not double down on prediction and control.
The future of the system is a function of the residue.

An environment can have too many elements or properties to measure, or taking measurements can be difficult or costly.
In such cases we can use a random sample of measurements as a representation of the environment's entire state.


## Attractor

An attractor is a state in the configuration space of a system that the dynamics of the system tend to move the system towards.
An attractor is a particular state that a system arrives in over and over again.
Attractors makes it possible to interact with complex systems in a predictable way.
Example human attractors: sleep, hunger, sadness, happiness.

Business systems have attractors.
It is beneficial to our understanding of the system to identify them.
Residuality theory provides methods for finding these attractors in a business system.
By looking for recurring states in the system as a whole instead of patterns in the constituent elements we no longer need to predict the detail of every individual element.
Attractors are a key idea in residuality theory.

An attractor is associated with one or more stressors, the stressors that when applied moves the system towards that attractor.

It is not possible to know all attractors, or how the system will move between them.


When architecting a system, do not try to predict the attractors.
Do not assume what attractors the system need and try to design for them.
The residuality process will help you find them.


## Stressor

Things that may happen to a business system.
A stressor is associated with an attractor.
This means that when the stressor is applied, the system tend to move towards a particular attractor.


## Criticality

(
TODO Introduce N, K, and P.
)

A system has reached criticality when the configuration of its constituent elements and links are such that the system becomes resilient to change.
A monolithic software system has new elements and few links.
A microservices based software has many elements and many links.
(
What is an example of a software system with many elements and few links, of few elements and many links?
)

Each element's behavior also influences criticality.
When the elements are restricted in the way they interact with the rest of the system the system as a whole becomes more resilient to changes.
This is called bias, bias towards particular behavior.
In software, some examples that increase bias are:
- the use of interfaces
- service-oriented ideas
- uniform error handling

The goal is to design a system so that they have few but strong attractors.
This means that a disturbance which brings the system state away from an attractor will soon finds it way back to an attractor.
As the number of elements and links in the system increases the number of attractors also increases, meaning there are more states that the system can come to rest at.
As the bias of the system increases the number of attractors decreases, meaning there are fewer states that the system can come to rest at.
We want to keep the number of attractors low since we want to be in control of which attractor states we have.

Software design principles often act to find the right balance in the number of attractors.
This includes SOLID, DRY, OOP, and many design patterns.
They do not describe what the right levels of number of elements, link, or node bias are.
One way to find those levels is to randomly simulate the environment until the architecture shows signs of criticality.

## Structuralism

A false belief that the disordered context in which a software system operates must have a rigid structure that can be discovered and solved by simple patterns.
It denies the reality of the disordered world.
May tools focus on mapping and capturing structure.
By mapping the structure it is believed that one can understand the system.
This can lead to analysis paralysis due to the large number of interacting elements.
The rate of change makes the mapping obsolete.

Structuralism cannot deal with the problems of time, uncertainty, and change.
The structure of a business system cannot be used to predict the future.

Software engineers have tried to use structuralism to describe social structures and language.
Requirements engineering in an example of trying to find structure in language.


## Coupling

Coupling is when the residue forced by a stressor impacts two components of the architecture.
Side effects caused by coupling is called contagion.


## Contagion

Side effects caused by coupling.
When a fix require change to multiple components, which makes it risky.
Changes ripple through the system, and risk bringing the entire architecture down.
This is called hyperliminal coupling, coupling that is not visible in the software architecture but is discovered during business environment simulation.
The presence of hyperliminal coupling reduces criticality.


# Residuality Theory

A way to describe the structure of a software application.
A way to build a software architecture design.
Focused around the effect of stress on a software design.
Allows software architects to engage with non-ergodic, hyperliminal systems in an honest way.

Claims that simulated stress is better than prediction, requirements analysis, reuse of patterns, and reactive change management by coding.
Those ideas should be abandoned, they are dogma from other older engineering disciplines.

Software systems should be thought of as a collection of residues.
The residuality technique is the result of studying architectures designed in fast moving unpredictable environments.
Where requirements are incomplete.

By stressing an architecture being designed it is possible to arrive at a solid, dependable, and well documented architecture.


## Residue

Residue is the left over pieces of a system after it is exposed to some form of stress and updated to survive that stress.
Old system → something happens in the non-ergodic context → the system fails → system is updated → {parts from the old system, newly created parts}.
Residue is the architectural change that brought the system from the original that could not survive the stressor / attractor to a new architecture that could.
A residue is a description of a change to the architecture.
Residue is the unit of software architecture.
Residue incorporates time, uncertainty, and change.
Residue is not a unit of a software program.
An architecture can be expressed as changing residues over time.

When the complex outer context of a hyperliminal system changes and the inner ergodic system changes in response, the new inner system is the residue of the old system.
The new system will contain a part that is a damaged version of the software architecture.


