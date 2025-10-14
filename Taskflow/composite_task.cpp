// Project includes.
#include "utils.h"

// Taskflow includes.
#include <taskflow/taskflow.hpp>

// Standard library includes.
#include <iostream>


int main()
{
	tf::Taskflow f1, f2;

	f1.name("Inner Taskflow");
	f2.name("Outer Taskflow");

	// create taskflow f1 of two tasks
	tf::Task f1A = f1.emplace([]() { std::cout << "Task f1A\n"; }).name("f1A");
	tf::Task f1B = f1.emplace([]() { std::cout << "Task f1B\n"; }).name("f1B");

	// create taskflow f2 with one module task composed of f1
	tf::Task f2A = f2.emplace([]() { std::cout << "Task f2A\n"; }).name("f2A");
	tf::Task f2B = f2.emplace([]() { std::cout << "Task f2B\n"; }).name("f2B");
	tf::Task f2C = f2.emplace([]() { std::cout << "Task f2C\n"; }).name("f2C");
	tf::Task f1_module_task = f2.composed_of(f1).name("module");

	f1_module_task.succeed(f2A, f2B).precede(f2C);

	dumpToFile(f2, "composite_task.dot");
}
