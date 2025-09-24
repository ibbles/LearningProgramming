// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <cstring>
#include <ios>
#include <filesystem>
#include <fstream>

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