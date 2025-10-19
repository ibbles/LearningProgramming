// Project includes
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <array>
#include <cmath>
#include <iomanip>
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

void init_A()
{
	// Make diagonal larger to get a better condition number.
	A[0][0] = next_double() + next_double();
	A[0][1] = next_double();
	A[1][0] = next_double();
	A[1][1] = next_double() + next_double();
}

void init_x()
{
	x[0] = 0.0;
	x[1] = 0.0;
}

void read_b()
{
	std::cout << "b[0]? ";
	std::cin >> b[0];
	std::cout << "b[1]? ";
	std::cin >> b[1];
}

void compute_residual()
{
	r[0] = (A[0][0] * x[0] + A[0][1] * x[1]) - b[0];
	r[1] = (A[1][0] * x[0] + A[1][1] * x[1]) - b[1];
}

void record_trajectory()
{
	trajectory.push_back(Step{{x[0], x[1]}, {r[0], r[1]}});
}

double norm(const double (&v)[2])
{
	return std::sqrt((v[0] * v[0]) + (v[1] * v[1]));
}

double get_residual_norm()
{
	return norm(r);
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

	std::cout << std::setprecision(4);
	std::cout << "| " << A[0][0] << ", " << A[0][1] << "| " << x[0] << " = " << b[0] << "\n";
	std::cout << "| " << A[1][0] << ", " << A[1][1] << "| " << x[1] << " = " << b[1] << "\n";
	std::cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << '\n';
	std::cout << "Ax[0] = " << Ax[0] << ", b[0] = " << b[0] << '\n';
	std::cout << "Ax[1] = " << Ax[1] << ", b[1] = " << b[1] << '\n';
	std::cout << "r[0] = " << r[0] << ", r[1] = " << r[1] << '\n';
	std::cout << "Residual norm: " << get_residual_norm() << '\n';
	std::cout << "Iterations: " << num_iterations << '\n';
	std::cout << "Trajectory: \n";
	for (const Step& step : trajectory)
	{
		std::cout << "  (" << step.x[0] << ", " << step.x[1] << "), |r| = " << norm(step.r) << '\n';
	}
}

int main()
{
	tf::Executor executor;

	tf::Taskflow taskflow;
	taskflow.name("Gauss-Seidel");

	tf::Task init_A = taskflow.emplace(::init_A);
	init_A.name("Init A");

	tf::Task init_x = taskflow.emplace(::init_x);
	init_x.name("Init x");

	tf::Task read_b = taskflow.emplace(::read_b);
	read_b.name("Read b");

	tf::Task compute_residual = taskflow.emplace(::compute_residual);
	compute_residual.name("Compute Residual");

	tf::Task record_trajectory = taskflow.emplace(::record_trajectory);
	record_trajectory.name("Record Trajectory");

	tf::Task should_loop = taskflow.emplace(
		[]()
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
		});
	should_loop.name("Should loop");

	tf::Task update_x = taskflow.emplace(::update_x);
	update_x.name("Update x");

	tf::Task print_result = taskflow.emplace(::print_result);
	print_result.name("Print result");

	compute_residual.succeed(init_A, init_x, read_b);
	compute_residual.precede(record_trajectory);
	record_trajectory.precede(should_loop);
	should_loop.precede(update_x, print_result);
	update_x.precede(compute_residual);

	dumpToFile(taskflow, "gauss-seidel.dot");

	executor.run(taskflow).wait();
}
