#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <unordered_set>

#include <benchmark/benchmark.h>

namespace fs = std::filesystem;

static
std::ifstream
SetupInputStream(
	benchmark::State& state)
{
	std::ifstream is{ R"-(E:\Utilisateurs\Arnaud\Documents\GitHub\CppSandbox\Ressources\liste_francais.txt)-" };

	if (!is)
		state.SkipWithError("File not found !");

	return is;
}

static
void
BM_LinearSearch(
	benchmark::State& state)
{	
	auto is = SetupInputStream(state);

	std::vector<std::string> vec;

	std::copy(
		std::istream_iterator<std::string>{ is },
		std::istream_iterator<std::string>{},
		std::back_inserter(vec));

	for (auto _ : state)
	{
		auto it = std::find(vec.begin(), vec.end(), "necessaire");

		benchmark::DoNotOptimize(it);
	}
}
// Register the function as a benchmark
BENCHMARK(BM_LinearSearch);

static
void
BM_BinarySearch(
	benchmark::State& state)
{
	auto is = SetupInputStream(state);

	std::vector<std::string> vec;

	std::copy(
		std::istream_iterator<std::string>{ is },
		std::istream_iterator<std::string>{},
		std::back_inserter(vec));

	for (auto _ : state)
	{
		auto it = std::lower_bound(vec.begin(), vec.end(), "necessaire");

		benchmark::DoNotOptimize(it);
	}
}
// Register the function as a benchmark
BENCHMARK(BM_BinarySearch);

static
void
BM_StdSet(
	benchmark::State& state)
{
	auto is = SetupInputStream(state);

	std::set<std::string> set;

	std::istream_iterator<std::string> it{ is }, end{};

	for (; it != end; ++it)
		set.emplace(*it);

	for (auto _ : state)
	{
		auto it = set.find("necessaire");

		benchmark::DoNotOptimize(it);
	}
}
// Register the function as a benchmark
BENCHMARK(BM_StdSet);

static
void
BM_StdUnorderedSet(
	benchmark::State& state)
{
	auto is = SetupInputStream(state);

	std::unordered_set<std::string> set;

	std::istream_iterator<std::string> it{ is }, end{};

	for (; it != end; ++it)
		set.emplace(*it);

	for (auto _ : state)
	{
		auto it = set.find("necessaire");

		benchmark::DoNotOptimize(it);
	}
}
// Register the function as a benchmark
BENCHMARK(BM_StdUnorderedSet);