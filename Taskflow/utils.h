// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <cstring>
#include <ios>
#include <filesystem>
#include <fstream>

inline void printTaskflow(tf::Taskflow& taskflow)
{
	for (auto& node_it : taskflow.graph())
	{
		auto* node = node_it.get();
		std::cout << "Node " << node->name() << ":\n";
		std::cout << "  Predeciessors: " << node->num_predecessors() << '\n';
		std::cout << "  Successors: " << node->num_successors() << '\n';
		std::cout << "  Strong dependencies: " << node->num_strong_dependencies() << '\n';
		std::cout << "  Weak dependencies: " << node->num_weak_dependencies() << "\n";
	}
}

inline void dumpToFile(tf::Taskflow& taskflow, std::filesystem::path path)
{
	errno = 0;
	std::ofstream stream(path, std::ios_base::trunc);
	if (!stream)
	{
		std::cerr << "utils > dumpToFile: Could not open " << path << ": " << strerror(errno);
		return;
	}

	taskflow.dump(stream);
}
