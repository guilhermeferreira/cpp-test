#include "persistence/IniFormat.hpp"

#include <sstream>

#include <klee/klee.h>

int main()
{
    std::string key;
    klee_make_symbolic(&key, sizeof(key), "key");

	std::stringstream contents;
	std::stringstream ss;
	ss << key << "=value";
	contents << ss.str();

	IniFormat format;
	std::map<std::string, std::string> settings = format.load(contents);

    return 0;
}

#if 0

namespace test::performance {

//--------------------------------------------------------------------------

static void IniFormatBenchmark_BM_load(benchmark::State& state)
{
	// Setup code. Create N keys to load. Required to calculate algorithmic
	// complexity
	std::stringstream contents;
	std::size_t keyCount = state.range(0);
	for (int keyIdx = 0; keyIdx < keyCount; keyIdx++) {
		std::stringstream ss;
		ss << "Key" << keyIdx << "=Value" << keyIdx;
		contents << ss.str();
	}

	IniFormat format;
	std::map<std::string, std::string> settings;
	for (auto _: state) {
		// Benchmake code
		settings = format.load(contents);
	}

	// Asymptotic Complexity (Big O) for the given problem size
	state.SetComplexityN(keyCount);
}

BENCHMARK(IniFormatBenchmark_BM_load)
	->RangeMultiplier(10)
	->Range(10, 1000)
	->Complexity();

//--------------------------------------------------------------------------

static void IniFormatBenchmark_BM_store(benchmark::State& state)
{
	// Setup code. Create N keys to store. Required to calculate algorithmic
	// complexity
	std::map<std::string, std::string> settings;
	std::size_t keyCount = state.range(0);
	for (int keyIdx = 0; keyIdx < keyCount; keyIdx++) {
		std::stringstream key;
		key << "Key" << keyIdx;

		std::stringstream value;
		value << "Value" << keyIdx;

		settings[key.str()] = value.str();
	}

	IniFormat format;
	std::stringstream contents;
	for (auto _: state) {
		// Benchmake code
		format.store(settings, contents);
	}

	// Asymptotic Complexity (Big O) for the given problem size
	state.SetComplexityN(keyCount);
}

BENCHMARK(IniFormatBenchmark_BM_store)
	->RangeMultiplier(10)
	->Range(10, 1000)
	->Complexity();

//--------------------------------------------------------------------------

static void IniFormatBenchmark_BM_read(benchmark::State& state)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";

	IniFormat format;
	for (auto _: state) {
		format.read(contents);
	}
}

BENCHMARK(IniFormatBenchmark_BM_read);

//--------------------------------------------------------------------------

static void IniFormatBenchmark_BM_parse(benchmark::State& state)
{
	std::stringstream contents;
	contents << "Key1=Value1\n";

	IniFormat format;
	for (auto _: state) {
		format.parse("Key1=Value1");
	}
}

BENCHMARK(IniFormatBenchmark_BM_parse);

//--------------------------------------------------------------------------

} // test::performance

#endif
