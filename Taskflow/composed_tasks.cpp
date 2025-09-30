// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>
#include <taskflow/core/executor.hpp>
#include <taskflow/core/taskflow.hpp>


tf::Taskflow buildTaskflow1()
{
	tf::Taskflow taskflow;
	taskflow.emplace(
		[]() { std::cout << "Flow 1 Task 1\n"; },
		[]() { std::cout << "Flow 1 Task 2\n"; },
		[]() { std::cout << "Flow 1 Task 3\n"; });
	return taskflow;
}

tf::Taskflow buildTaskflow2()
{
	tf::Taskflow taskflow;
	taskflow.emplace(
		[]() { std::cout << "Flow 2 Task 1\n"; },
		[]() { std::cout << "Flow 2 Task 2\n"; },
		[]() { std::cout << "Flow 2 Task 3\n"; });
	return taskflow;
}

int  main()
{
	tf::Taskflow taskflow;
	tf::Taskflow taskflow1 = buildTaskflow1();
	tf::Taskflow taskflow2 = buildTaskflow2();
	tf::Task task1 = taskflow.composed_of(taskflow1);
	tf::Task task2 = taskflow.composed_of(taskflow2);
	task1.precede(task2);
	tf::Executor().run(taskflow).wait();
	dumpToFile(taskflow, "composed_tasks.dot");
}
