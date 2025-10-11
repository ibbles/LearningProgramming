// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

static int counter {0};
static int goal {10};

void start_1() { std::cout << "Start 1\n"; }
void start_2() { std::cout << "Start 2\n"; }
void increment_counter() { ++counter; };
int is_goal_reached() { return counter >= goal; }
void print_counter() { std::cout << counter << '\n'; }

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	tf::Task start_1 = taskflow.emplace(::start_1);
	tf::Task start_2 = taskflow.emplace(::start_2);
	tf::Task increment_counter = taskflow.emplace(::increment_counter);
	tf::Task is_goal_reached = taskflow.emplace(::is_goal_reached);
	tf::Task print_counter = taskflow.emplace(::print_counter);

	increment_counter.succeed(start_1, start_2);
	increment_counter.precede(is_goal_reached);
	is_goal_reached.precede(increment_counter, print_counter);
	executor.run(taskflow).wait();

	taskflow.name("Multi-Dependent Loop Start");
	start_1.name("Start 1");
	start_2.name("Start 2");
	increment_counter.name("Increment Counter");
	is_goal_reached.name("Is Goal Reached");
	print_counter.name("Print Counter");
	dumpToFile(taskflow, "multi_predecessor_loop_start.dot");
}