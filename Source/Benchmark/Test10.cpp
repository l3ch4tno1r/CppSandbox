#include <iostream>
#include <benchmark/benchmark.h>

static
void
BM_StringCreation(
    benchmark::State& state)
{
    for (auto _ : state)
        std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

static
void
BM_StringDeepCopy(
    benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str1{ "Hello world ! Making that string long enough to avoid short string optimisation" };
        std::string str2{ str1 };
    }
}
// Register the function as a benchmark
BENCHMARK(BM_StringDeepCopy);

static
void
BM_StringShallowCopy(
    benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str1{ "Hello world ! Making that string long enough to avoid short string optimisation" };
        std::string str2{ std::move(str1) };
    }
}
// Register the function as a benchmark
BENCHMARK(BM_StringShallowCopy);

BENCHMARK_MAIN();