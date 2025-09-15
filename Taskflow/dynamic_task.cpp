// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

int global_state {0};

void input_task()
{
	std::cout << "How many dynamic tasks? ";
    std::cin >> global_state;
}

// This task is not scheduled in main, instead it is
// scheduled by create_dynamic_tasks.
void dynamic_task()
{
    std::cout << "Dynamic task.\n";
}

void create_dynamic_tasks(tf::Subflow& subflow)
{
    for (int i = 0; i < global_state; ++i)
    {
        subflow.emplace(dynamic_task);
    }
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	tf::Task input = taskflow.emplace(input_task);
    tf::Task dynamic = taskflow.emplace(create_dynamic_tasks);
    input.precede(dynamic);
	executor.run(taskflow).wait();
	return 0;
}
