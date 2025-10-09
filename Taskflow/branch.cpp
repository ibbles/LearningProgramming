// Project includes.
#include "utils.h"

// Taskflow includes.
#include <taskflow/taskflow.hpp>

// Standard library includes.
#include <iostream>

static int number {0};

void read_data()
{
	std::cout << "Data? ";
	std::cin >> number;
}

int even_or_odd()
{
	return number % 2;
}

void print_even()
{
	std::cout << "Number " << number << " is even.\n";
}

void print_odd()
{
	std::cout << "Number " << number << " is odd.\n";
}

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	tf::Task read_data = taskflow.emplace(::read_data);
	tf::Task even_or_odd = taskflow.emplace(::even_or_odd);
	tf::Task print_even = taskflow.emplace(::print_even);
	tf::Task print_odd = taskflow.emplace(::print_odd);

	read_data.precede(even_or_odd);
	even_or_odd.precede(print_even, print_odd);

	executor.run(taskflow).wait();


	taskflow.name("Print Even Or Odd");
	read_data.name("Read Data");
	even_or_odd.name("Even Or Odd");
	print_even.name("Print Even");
	print_odd.name("Print Odd");
	dumpToFile(taskflow, "branch.dot");

	return 0;
}
