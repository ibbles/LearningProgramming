// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	auto [t1, t2, t3, t4, t5, t6] = taskflow.emplace(
		[](){}, [](){}, [](){}, [](){}, [](){}, [](){});
	t1.precede(t2, t3, t4);
	t2.precede(t5);
	t3.precede(t5);
	t4.precede(t6);
	t5.precede(t6);
	executor.run(taskflow).wait();

	taskflow.name("Example Task Graph");
	t1.name("Task 1");
	t2.name("Task 2");
	t3.name("Task 3");
	t4.name("Task 4");
	t5.name("Task 5");
	t6.name("Task 6");
	dumpToFile(taskflow, "example_task_graph.dot");

	return 0;
}
