// Project includes.
#include "utils.h"

// Taskflow includes.
#include <taskflow/taskflow.hpp>

// Standard library includes.
#include <iostream>

static int counter {0};
static int goal {10};

void read_goal()
{
	std::cout << "Goal? ";
	std::cin >> goal;
}

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

	tf::Task read_goal = taskflow.emplace(::read_goal);
	tf::Task increment_counter = taskflow.emplace(::increment_counter);
	tf::Task print_counter = taskflow.emplace(::print_counter);
	tf::Task is_goal_reached = taskflow.emplace(::is_goal_reached);
	tf::Task done = taskflow.emplace([](){});

	read_goal.precede(increment_counter);
	increment_counter.precede(print_counter);
	print_counter.precede(is_goal_reached);
	is_goal_reached.precede(increment_counter, done);

	read_goal.name("Read Goal");
	taskflow.name("Counter");
	increment_counter.name("Increment Counter");
	print_counter.name("Print Counter");
	is_goal_reached.name("Is Goal Reached");
	done.name("Done");
	dumpToFile(taskflow, "counter.dot");

	executor.run(taskflow).wait();

	return 0;
}
