#include "utils.h"
#include "taskflow/taskflow.hpp"

class Space
{
public:
	void emplaceTasks(tf::Taskflow& taskflow);

	void broadPhase();
	void nearPhase(tf::Subflow& subflow);
	void nearPhaseSphereSphere();
	void nearPhaseSphereBox();
	void nearPhaseBoxBox();

private:
	int m_numSphereSperePairs {0};
	int m_numSphereBoxPairs {0};
	int m_numBoxBoxPairs {0};
};

void Space::emplaceTasks(tf::Taskflow& taskflow)
{
	tf::Task broadPhaseTask = taskflow.emplace([this]() { broadPhase(); });
	broadPhaseTask.name("Broad-Phase");
	tf::Task nearPhasesTask = taskflow.emplace([this](tf::Subflow& subflow) { nearPhase(subflow); });
	nearPhasesTask.name("Near-Phase");
	broadPhaseTask.precede(nearPhasesTask);
}

void Space::broadPhase()
{
	m_numSphereSperePairs = 1;
	m_numSphereBoxPairs = 0;
	m_numBoxBoxPairs = 1;
}

void Space::nearPhase(tf::Subflow& subflow)
{
	subflow.retain(true);
	if (m_numSphereSperePairs > 0)
		subflow.emplace([this]() { nearPhaseSphereSphere(); }).name("Sphere-Sphere");
	if (m_numSphereBoxPairs > 0)
		subflow.emplace([this]() { nearPhaseSphereBox(); }).name("Sphere-Box");
	if (m_numBoxBoxPairs > 0)
		subflow.emplace([this]() { nearPhaseBoxBox(); }).name("Box-Box");
}

void Space::nearPhaseSphereSphere()
{
}
void Space::nearPhaseSphereBox()
{
}
void Space::nearPhaseBoxBox()
{
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;
	taskflow.name("Dynamic Tasks");

	Space space;
	space.emplaceTasks(taskflow);
	executor.run(taskflow).wait();

	dumpToFile(taskflow, "work_if_needed.dot");
}
