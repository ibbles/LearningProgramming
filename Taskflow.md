Taskflow is a task graph computing system.
This means that instead of chaining functions together as the program is executed the structure of the program is defined up-front in a task that is executed by a runtime.
Taskflow is an API for defining task graphs and a runtime for executing them.
These two concepts are codified in two types:
- `tf::Taskflow`: A task graph.
- `tf::Executor`: A runtime for executing task graphs.

Taskflow provides a number of different task types with different features and characteristics.
- Static task: Basically just a callback.
- Dynamic task: A task that creates child-tasks at runtime.
- Composite task: The inclusion of one `tf::Taskflow` into another.
- Condition task: Runtime selection of which task should be executed next.

# A Simple Example

Let's look at the simplest possible example, a task graph that contains a single task that calls a function.
```cpp
// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void task() { std::cout << "A task.\n"; }

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.emplace(task);
	executor.run(taskflow).wait();
	return 0;
}
```

We can see the central parts of a Taskflow program:
- A task to perform
	- In this case the task is to print `A task.` and the implementation of the task is a function named `task`.
- An instance of the `tf::Executor` class, here named `executor`.
- An instance of the `tf::Taskflow` class, here named `taskflow`.
- The `task` function being emplaced into the taskflow.
- The executor running the taskflow and waiting for it to complete.

# Creating Tasks

A task is any callable, such as a regular function, that does not take any arguments.
To create an actual task we call the `emplace` member function of the `tf::Taskflow` that should  own the task.
This creates a `tf::Task` instance inside the `tf::Taskflow`.
When a `tf::Taskflow` run by an executor then the tasks within that `tf::Taskflow`  is executed.
The return value of `tf::Taskflow::emplace` is one or more `tf::Task`.
The `tf::Task` objects can be used to configure the created task, for example to add dependencies to other tasks.

# Creating Task Dependencies

Tasks can either precede or succeed other tasks.
A task that precede another task will run to completion before that other task starts.
A task that succeeds another task will not start until that other task has run to completion.
We create these dependencies using member functions on `tf::Task`.
```cpp
// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void first_task() { std::cout << "First task.\n"; }
void second_task() { std::cout << "Second task.\n"; }

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	tf::Task first_task = taskflow.emplace(::first_task);
	tf::Task second_task = taskflow.emplace(::second_task);
	first_task.precede(second_task);
	executor.run(taskflow).wait();
	return 0;
}
```

In this case we could achieve the same thing using `second_task.succeed(first_task);` instead.

# Creating Multiple Tasks

We can pass multiple callables to `tf::Taskflow::emplace`.
This causes multiple `tf::Task` objects to be created and returned in a tuple.
C++ structured bindings doesn't allow us to name the type of the elements in the tuple, so we have to use `auto` instead of `tf::Task`.

In the following we create a setup task, a number of parallel tasks, and a teardown task.
Dependencies are set up so that the setup task is run before all parallel tasks and the teardown task is run after all parallel tasks.

```cpp
// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void setup_task() {	std::cout << "Setup task.\n"; }
void parallel_task_1() { std::cout << "Parallel task 1.\n"; }
void parallel_task_2() { std::cout << "Parallel task 2.\n"; }
void parallel_task_3() { std::cout << "Parallel task 3.\n"; }
void teardown_task() { std::cout << "Teardown task.\n"; }

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	auto [setup, parallel_1, parallel_2, parallel_3, teardown] = taskflow.emplace(
		::setup_task, ::parallel_task_1, ::parallel_task_2, ::parallel_task_3, ::teardown_task);
	setup.precede(parallel_1, parallel_2, parallel_3);
	teardown.succeed(parallel_1, parallel_2, parallel_3);
	executor.run(taskflow).wait();
	return 0;
}
```


# Creating Dynamic Tasks

So far we have created static tasks, meaning that we know exactly what is going to be executed when `tf::Executor::run` is called.
Dynamic tasks lets us defer the task creation until runtime, when parts of the task graph has already been executed.
So an early task can communicate with a later dynamic task and influence the tasks that are created.

The following exemplifies this with a dynamic task that is run as many times as the user requested in the input task.
Notice that no synchronization on  the `global_state` variable is needed, Taskflow ensures that there is a synchronization between the tasks with dependencies.

```cpp
// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

int global_state {0};

void input_task()
{
	std::cout << "How many dynamic tasks? ";
    std::cin >> global_state;
}

// This task is not scheduled in main, instead it is
// scheduled by create_dynamic_tasks.
void dynamic_task()
{
    std::cout << "Dynamic task.\n";
}

void create_dynamic_tasks(tf::Subflow& subflow)
{
    for (int i = 0; i < global_state; ++i)
    {
        subflow.emplace(dynamic_task);
    }
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	tf::Task input = taskflow.emplace(input_task);
    tf::Task dynamic = taskflow.emplace(create_dynamic_tasks);
    input.precede(dynamic);
	executor.run(taskflow).wait();
	return 0;
}
```

# References

- 1: [_Taskflow: A Lightweight Parallel and Heterogeneous Task Graph Computing System_ by Tsun-Wei Huang et.al. 2022 @ taskflow.github.io](https://taskflow.github.io/papers/tpds21-taskflow.pdf)
