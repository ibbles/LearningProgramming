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
	taskflow.emplace(task);
	executor.run(taskflow).wait();
	return 0;
}
