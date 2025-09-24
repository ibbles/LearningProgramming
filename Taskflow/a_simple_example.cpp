// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void task()
{
	std::cout << "A task.\n";
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("A Simple Example");
	tf::Task t = taskflow.emplace(task);
	t.name("A task");
	executor.run(taskflow).wait();
	dumpToFile(taskflow, "a_simple_example.dot");
	return 0;
}
