// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void first_task()
{
	std::cout << "First task.\n";
}

void second_task()
{
	std::cout << "Second task.\n";
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	tf::Task first_task = taskflow.emplace(::first_task);
	tf::Task second_task = taskflow.emplace(::second_task);
	first_task.precede(second_task);
	executor.run(taskflow).wait();

	taskflow.name("Creating Dependencies");
	first_task.name("First Task");
	second_task.name("Second Task");
	dumpToFile(taskflow, "creating_dependencies.dot");

	return 0;
}
