// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void setup_task()
{
	std::cout << "Setup task.\n";
}

void parallel_task_1()
{
	std::cout << "Parallel task 1.\n";
}

void parallel_task_2()
{
	std::cout << "Parallel task 2.\n";
}

void parallel_task_3()
{
	std::cout << "Parallel task 3.\n";
}

void teardown_task()
{
	std::cout << "Teardown task.\n";
}

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
