#include "taskflow/taskflow.hpp"
#include <iostream>
#include <fstream>

void work()
{
	std::cout << "Doing work.\n";
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("Simplest Possible Task Graph");
	tf::Task task = taskflow.emplace(work);
	task.name("A Lonely Task");
	std::ofstream file("simplest_possible_task_graph.dot");
	taskflow.dump(file);
	executor.run(taskflow).wait();
}
