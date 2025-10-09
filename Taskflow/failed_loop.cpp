// Project includes.
#include "utils.h"

// Taskflow includes.
#include <taskflow/taskflow.hpp>

// Standard library includes.
#include <iostream>

static int counter {0};
static int goal {10};

void increment_counter()
{
	++counter;
}

int is_goal_reached()
{
	return counter >= goal;
}

void print_counter()
{
	std::cout << "Counter is " << counter << ".\n";
}

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	tf::Task increment_counter = taskflow.emplace(::increment_counter);
	tf::Task is_goal_reached = taskflow.emplace(::is_goal_reached);
	tf::Task print_counter = taskflow.emplace(::print_counter);

	increment_counter.precede(is_goal_reached);
	is_goal_reached.precede(increment_counter, print_counter);

	//executor.run(taskflow).wait();


	taskflow.name("Failed Loop");
	increment_counter.name("Increment Counter");
	is_goal_reached.name("Is Goal Reached");
	print_counter.name("Print Counter");
	dumpToFile(taskflow, "failed_loop.dot");

	return 0;
}
