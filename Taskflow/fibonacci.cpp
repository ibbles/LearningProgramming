#include "utils.h"
#include "taskflow/taskflow.hpp"
#include <iostream>
#include <fstream>
#include <taskflow/core/executor.hpp>

int spawn(int n, tf::Subflow& subflow)
{
	subflow.retain(true);
	if (n < 2)
		return n;
	int result1, result2;
	tf::Task task1 = subflow.emplace([n, &result1](tf::Subflow& subsubflow) { result1 = spawn(n - 1, subsubflow); });
	tf::Task task2 = subflow.emplace([n, &result2](tf::Subflow& subsubflow) { result2 = spawn(n - 2, subsubflow); });
	subflow.join(); // Join to run the subflow immediately.
	task1.name("fib(" + std::to_string(n - 1) + ")=" + std::to_string(result1));
	task2.name("fib(" + std::to_string(n - 2) + ")=" + std::to_string(result2));
	lockedCout() << "spawn(" << n << ") returning " << result1 << " + " << result2 << " = " << (result1 + result2) << '\n';
	return result1 + result2;
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;

	int n {5};
	int result {0};
	tf::Task task = taskflow.emplace([n, &result](tf::Subflow& subflow) { result = spawn(n, subflow); });
	executor.run(taskflow).wait();
	task.name("fib(" + std::to_string(n) + ")=" + std::to_string(result));
	std::cout << "fib(" << n << ") = " << result << '\n';

	taskflow.name("Fibonacci");
	dumpToFile(taskflow, "fibonacci.dot");
}
