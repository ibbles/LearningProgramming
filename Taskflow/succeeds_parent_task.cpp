// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

// Written in 'input_task', read in 'dynamic_task'.
int global_state {0};

void input_work()
{
	std::cout << "How many dynamic tasks? ";
	std::cin >> global_state;
}

// This task is not scheduled in main, instead it is
// scheduled by create_dynamic_tasks.
void dynamic_work()
{
	std::cout << "Dynamic work.\n";
}

void create_dynamic_tasks(tf::Subflow& subflow)
{
	for (int i = 0; i < global_state; ++i)
	{
		tf::Task child = subflow.emplace(dynamic_work);
		child.name(std::string("Child ") + std::to_string(i));
	}

	subflow.retain(true);
}

void post_work()
{
	std::cout << "Post work.\n";
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	tf::Task input = taskflow.emplace(input_work);
	tf::Task dynamic = taskflow.emplace(create_dynamic_tasks);
	tf::Task post = taskflow.emplace(post_work);
	input.precede(dynamic);
	post.succeed(dynamic);
	executor.run(taskflow).wait();

	taskflow.name("Succeeds Parent Task");
	input.name("Input");
	dynamic.name("Dynamic");
	post.name("Post");
	dumpToFile(taskflow, "./succeeds_parent_task.dot");

	return 0;
}
