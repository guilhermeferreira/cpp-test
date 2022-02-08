#include "interface/Interface.hpp"

#include <benchmark/benchmark.h>

namespace test::performance {

//--------------------------------------------------------------------------

static void InterfaceBenchmark_BM_parse_set(benchmark::State& state)
{
	for (auto _: state) {
		Interface::parse("SET Key Value");
	}
}

BENCHMARK(InterfaceBenchmark_BM_parse_set);

//--------------------------------------------------------------------------

static void InterfaceBenchmark_BM_parse_get(benchmark::State& state)
{
	for (auto _: state) {
		Interface::parse("GET Key");
	}
}

BENCHMARK(InterfaceBenchmark_BM_parse_get);

//--------------------------------------------------------------------------

static void InterfaceBenchmark_BM_parse_delete(benchmark::State& state)
{
	for (auto _: state) {
		Interface::parse("DELETE Key");
	}
}

BENCHMARK(InterfaceBenchmark_BM_parse_delete);

//--------------------------------------------------------------------------

} // test::performance
