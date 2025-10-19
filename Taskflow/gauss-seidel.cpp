// Project includes
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <array>
#include <cmath>
#include <iomanip>
#include <ios>
#include <iostream>
#include <random>
#include <vector>

double A[2][2] {};
double x[2] {};
double b[2] {};
double r[2] {};

int num_iterations {};
constexpr int max_iterations {16};

struct Step
{
	double x[2];
	double r[2];
};

std::vector<Step> trajectory;

double next_double()
{
	static std::random_device dev;
	static std::mt19937 rng(dev());
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rng);
}

void random_A()
{
	// Make diagonal larger to get a better condition number.
	A[0][0] = next_double() + next_double();
	A[0][1] = next_double();
	A[1][0] = next_double();
	A[1][1] = next_double() + next_double();
}

void zero_x()
{
	x[0] = 0.0;
	x[1] = 0.0;
}

void read_b()
{
// Whether to use user-supplied values or hard-coded ones.
#if 1
	std::cout << "b[0]? ";
	std::cin >> b[0];
	std::cout << "b[1]? ";
	std::cin >> b[1];
#else
	b[0] = 2.0;
	b[1] = 8.0;
#endif
}

void compute_residual()
{
	r[0] = (A[0][0] * x[0] + A[0][1] * x[1]) - b[0];
	r[1] = (A[1][0] * x[0] + A[1][1] * x[1]) - b[1];
}

void record_trajectory()
{
	trajectory.push_back(Step {{x[0], x[1]}, {r[0], r[1]}});
}

double norm(const double (&v)[2])
{
	return std::sqrt((v[0] * v[0]) + (v[1] * v[1]));
}

double get_residual_norm()
{
	return norm(r);
}

int should_loop()
{
	constexpr int loop_again {0};
	constexpr int exit_loop {1};
	if (num_iterations >= max_iterations || get_residual_norm() < 1e-6)
	{
		return exit_loop;
	}
	else
	{
		return loop_again;
	}
}

int update_x()
{
	x[0] = (b[0] - A[0][1] * x[1]) / A[0][0];
	x[1] = (b[1] - A[1][0] * x[0]) / A[1][1];
	++num_iterations;
	return 0;
}

void print_result()
{
	// clang-format off
	double Ax[2] {
		A[0][0] * x[0] + A[0][1] * x[1],
		A[1][0] * x[0] + A[1][1] * x[1]
	};
	// clang-format on

	auto out = [](double v) -> const char*
	{
		std::cout << std::setw(7) << v;
		return "";
	};

	std::cout << std::setprecision(4) << std::fixed << std::left << std::setfill('0');
	std::cout << '\n';
	std::cout << "A:\n";
	std::cout << "  |" << out(A[0][0]) << ", " << out(A[0][1]) << "|\n";
	std::cout << "  |" << out(A[1][0]) << ", " << out(A[1][1]) << "|\n";
	std::cout << '\n';
	std::cout << "x:\n";
	std::cout << "  |" << out(x[0]) << "|\n";
	std::cout << "  |" << out(x[1]) << "|\n";
	std::cout << "\n";
	std::cout << "b:\n";
	std::cout << "  |" << out(b[0]) << "|\n";
	std::cout << "  |" << out(b[1]) << "|\n";
	std::cout << "\n";
	std::cout << "Ax = b:\n";
	std::cout << "  |" << out(A[0][0]) << ", " << out(A[0][1]) << "| |" << out(x[0]) << "| = |" << out(b[0]) << "|\n";
	std::cout << "  |" << out(A[1][0]) << ", " << out(A[1][1]) << "| |" << out(x[1]) << "| = |" << out(b[1]) << "|\n";
	std::cout << '\n';
	std::cout << "Ax = b:\n";
	std::cout << "  |" << Ax[0] << "| = |" << b[0] << "|\n";
	std::cout << "  |" << Ax[1] << "| = |" << b[1] << "|\n";
	std::cout << '\n';
	std::cout << "Ax - b:\n";
	std::cout << "  |" << r[0] << "|\n";
	std::cout << "  |" << r[1] << "|\n";
	std::cout << '\n';
	std::cout << "Residual norm: " << std::scientific << get_residual_norm() << '\n';
	std::cout << "Iterations: " << num_iterations << '\n';
	std::cout << "Trajectory: \n";
	for (const Step& step : trajectory)
	{
		std::cout << std::fixed << "  (" << out(step.x[0]) << ", " << out(step.x[1]) << "), |r| = " << std::scientific << norm(step.r) << '\n';
	}
}

int main()
{
	tf::Executor executor;
	tf::Taskflow taskflow;

	tf::Task random_A = taskflow.emplace(::random_A);
	tf::Task zero_x = taskflow.emplace(::zero_x);
	tf::Task read_b = taskflow.emplace(::read_b);
	tf::Task compute_residual = taskflow.emplace(::compute_residual);
	tf::Task record_trajectory = taskflow.emplace(::record_trajectory);
	tf::Task should_loop = taskflow.emplace(::should_loop);
	tf::Task update_x = taskflow.emplace(::update_x);
	tf::Task print_result = taskflow.emplace(::print_result);

	compute_residual.succeed(random_A, zero_x, read_b);
	compute_residual.precede(record_trajectory);
	record_trajectory.precede(should_loop);
	should_loop.precede(update_x, print_result);
	update_x.precede(compute_residual);

	taskflow.name("Gauss-Seidel");
	random_A.name("Init A");
	zero_x.name("Init x");
	read_b.name("Read b");
	compute_residual.name("Compute Residual");
	record_trajectory.name("Record Trajectory");
	should_loop.name("Should loop");
	update_x.name("Update x");
	print_result.name("Print result");
	dumpToFile(taskflow, "gauss-seidel.dot");

	executor.run(taskflow).wait();
}
