#include "taskflow/taskflow.hpp"
#include <iostream>
#include <taskflow/core/executor.hpp>
#include <taskflow/core/taskflow.hpp>

class A
{
public:
	void createUpdateTask(tf::Taskflow& taskflow)
	{
		// I don't know how to use 'std::bind'.
		// The following gives:
		//   error: call to member function 'emplace' is ambiguous
		#if 0
		taskflow.emplace(std::bind(&A::prepareWork, this));
		#endif

		tf::Task prepareWorkTask = taskflow.emplace([this](){prepareWork();});
		tf::Task workTask = taskflow.emplace([this](tf::Subflow& subflow){work(subflow);});
		prepareWorkTask.precede(workTask);
	}

private:
	void prepareWork()
	{
		std::cout << "A::work\n";
		m_haveWorkType2 = true;
	}

	void work(tf::Subflow& subflow)
	{
		if (m_haveWorkType1)
			subflow.emplace([this](){ work1(); });
		if (m_haveWorkType2)
			subflow.emplace([this](){ work2(); });
		if (m_haveWorkType3)
			subflow.emplace([this](){ work3(); });

	}

	void work1()
	{
		std::cout << "Work 1\n";
	}

	void work2()
	{
		std::cout << "Work 2\n";
	}

	void work3()
	{
		std::cout << "Work 3\n";
	}

private:
	bool m_haveWorkType1 {false};
	bool m_haveWorkType2 {false};
	bool m_haveWorkType3 {false};
};

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	A a;
	a.createUpdateTask(taskflow);
	executor.run(taskflow).wait();
}