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

void print_counter()
{
	std::cout << counter << " / " << goal << std::endl;
}

int is_goal_reached()
{
	return counter == goal;
}

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	tf::Task increment_counter = taskflow.emplace(::increment_counter);
	tf::Task print_counter = taskflow.emplace(::print_counter);
	tf::Task is_goal_reached = taskflow.emplace(::is_goal_reached);
	tf::Task done = taskflow.placeholder();

	increment_counter.precede(print_counter);
	print_counter.precede(is_goal_reached);
	is_goal_reached.precede(increment_counter, done);

	// Cannot run this taskflow because it doesn't have any root tasks.
	// increment_counter is not a root task because even weak dependencies
	// count when determining if a task is a root task.
	//executor.run(taskflow).wait();

	taskflow.name("Failed Loop");
	increment_counter.name("Increment Counter");
	print_counter.name("Print Counter");
	is_goal_reached.name("Is Goal Reached");
	done.name("Done");
	dumpToFile(taskflow, "failed_loop.dot");

	return 0;
}
