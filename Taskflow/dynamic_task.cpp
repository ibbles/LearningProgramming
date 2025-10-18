// Project includes.
 #include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <fstream>
#include <iostream>
#include <taskflow/core/executor.hpp>

#if 0

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;

	tf::Task A = taskflow.emplace([]() {}).name("A");
	tf::Task C = taskflow.emplace([]() {}).name("C");
	tf::Task D = taskflow.emplace([]() {}).name("D");

	tf::Task B = taskflow.emplace(
		 [](tf::Subflow& subflow)
		 {
			 tf::Task B1 = subflow.emplace([]() {}).name("B1");
			 tf::Task B2 = subflow.emplace([]() {}).name("B2");
			 tf::Task B3 = subflow.emplace([]() {}).name("B3");
			 B3.succeed(B1, B2); // B3 runs after B1 and B2
			 subflow.retain(true);
		 })
		.name("B");

	A.precede(B, C); // A runs before B and C
	D.succeed(B, C); // D runs after  B and C

	executor.run(taskflow).wait();

	std::ofstream file("dynamic_task.dot");
	taskflow.dump(file);
}
#elif 0

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("Taskflow");
	tf::Task parent =
		taskflow.emplace([](tf::Subflow& sf) { sf.emplace([]() { std::cout << "child\n"; }); });
	executor.run(taskflow).wait();
	// taskflow.dump(std::cout);
	std::ofstream file("dynamic_task.dot");
	taskflow.dump(file);
}

#elif 0
int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("Taskflow");

	tf::Task pre = taskflow.emplace([]() {});
	pre.name("Pre");
	tf::Task parent = taskflow.emplace(
		[](tf::Subflow& sf)
		{
			tf::Task child1 = sf.emplace([]() {});
			child1.name("Child1");

			tf::Task child2 = sf.emplace([]() {});
			child2.name("Child2");

			child1.precede(child2);
		});
	parent.name("Parent");
	tf::Task post = taskflow.emplace([]() {});
	post.name("Post");

	pre.precede(parent);
	parent.precede(post);

	executor.run(taskflow).wait();

	taskflow.dump(std::cout);

	std::ofstream file("dynamic_task.dot");
	taskflow.dump(file);
}

#else

// Written in 'input_task', read in 'dynamic_task'.
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
	subflow.retain(true);
	for (int i = 1; i <= global_state; ++i)
	{
		subflow.emplace(dynamic_task)
			.name("Child " + std::to_string(i));
	}
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("Dynamic Tasks");
	tf::Task input = taskflow.emplace(input_task).name("Input");
	tf::Task dynamic = taskflow.emplace(create_dynamic_tasks).name("Dynamic");
	input.precede(dynamic);
	executor.run(taskflow).wait();

	dumpToFile(taskflow, "dynamic_task.dot");

	return 0;
}

#endif
