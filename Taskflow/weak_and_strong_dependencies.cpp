// Project includes.
#include "utils.h"

// Taskflow includes.
#include <chrono>
#include <taskflow/taskflow.hpp>

// Standard library includes.
#include <iostream>
#include <thread>

void start_1()
{
	std::cout << "Start 1 start\n";
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Start 1 end\n";
}

void start_2()
{
	std::cout << "Start 2\n";
}

int condition()
{
	std::cout << "Condition\n";
	return 0;
}

void end_1()
{
	std::cout << "End 1 start\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "End 1 end\n";
}

void end_2()
{
	std::cout << "End 2\n";
}

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	tf::Task start_1 = taskflow.emplace(::start_1);
	tf::Task start_2 = taskflow.emplace(::start_2);
	tf::Task condition = taskflow.emplace(::condition);
	tf::Task end_1 = taskflow.emplace(::end_1);
	tf::Task end_2 = taskflow.emplace(::end_2);


	start_1.precede(end_1);
	start_2.precede(condition);
	condition.precede(end_1, end_2);

	taskflow.name("Weak And Strong Dependencies");
	start_1.name("Start 1");
	start_2.name("Start 2");
	condition.name("Condition");
	end_1.name("End 1");
	end_2.name("End 2");
	dumpToFile(taskflow, "weak_and_strong_dependencies.dot");

	for (auto& node_it : taskflow.graph())
	{
		auto* node = node_it.get();
		std::cout << "Node " << node->name() << ":\n";
		std::cout << "  Predeciessors: " << node->num_predecessors() << '\n';
		std::cout << "  Successors: " << node->num_successors() << '\n';
		std::cout << "  Strong dependencies: " << node->num_strong_dependencies() << '\n';
		std::cout << "  Weak dependencies: " << node->num_weak_dependencies() << "\n";
	}

	executor.run(taskflow).wait();

	return 0;
}
