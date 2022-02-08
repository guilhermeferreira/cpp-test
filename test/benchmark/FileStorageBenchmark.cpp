#include "persistence/FileStorage.hpp"

#include <sstream>

#include <benchmark/benchmark.h>

namespace test::performance {

//--------------------------------------------------------------------------

class FileStorageBenchmarkFixture : public benchmark::Fixture
{
public:
	FileStorageBenchmarkFixture()
	: _value1{}
	, _value2{}
	, _value3{}
	, _value4{}
	{
	}

	void SetUp(const benchmark::State& state) override {
		FileStorage storage{ TEST_DST_DIR "database.ini" };

		storage.load();

		// Save original values
		_value1 = storage.get("Key1");
		_value2 = storage.get("Key2");
		_value3 = storage.get("Key3");
		_value4 = storage.get("Key4");
	}

	void TearDown(const benchmark::State& state) override {
		FileStorage storage{ TEST_DST_DIR "database.ini" };

		// Restore original values
		storage.set("Key1", _value1);
		storage.set("Key2", _value2);
		storage.set("Key3", _value3);
		storage.set("Key4", _value4);

		storage.store();
	}

private:
	std::string _value1;
	std::string _value2;
	std::string _value3;
	std::string _value4;

};

//--------------------------------------------------------------------------

BENCHMARK_DEFINE_F(FileStorageBenchmarkFixture, BM_load)(benchmark::State &state)
{
	std::size_t keyCount = state.range(0);
	{
		// Setup code. Create N keys to store and them load. Required to calculate
		// algorithmic complexity
		FileStorage storage{ TEST_DST_DIR "database.ini" };
		for (int keyIdx = 0; keyIdx < keyCount; keyIdx++) {
			std::stringstream ss;
			ss << "Key" << keyIdx;
			std::string key = ss.str();
			storage.set(key, "Value");
		}
		storage.store();
	}
	
	FileStorage storage{ TEST_DST_DIR "database.ini" };
	for (auto _: state) {
		// Benchmake code
		storage.load();
	}

	// Asymptotic Complexity (Big O) for the given problem size
	state.SetComplexityN(keyCount);
}

BENCHMARK_REGISTER_F(FileStorageBenchmarkFixture, BM_load)
	->Arg(10)
	->Arg(100)
	->Arg(1000)
	->Arg(10000)
	->Arg(100000)
	->Complexity();

//--------------------------------------------------------------------------

BENCHMARK_DEFINE_F(FileStorageBenchmarkFixture, BM_store)(benchmark::State &state)
{
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	// Setup code. Create N keys to store. Required to calculate algorithmic
	// complexity
	std::size_t keyCount = state.range(0);
	for (int keyIdx = 0; keyIdx < keyCount; keyIdx++) {
		std::stringstream ss;
		ss << "Key" << keyIdx;
		std::string key = ss.str();
		storage.set(key, "Value");
	}

	for (auto _: state) {
		// Benchmake code
		storage.store();
		// Flush cache
		benchmark::ClobberMemory();
	}

	// Asymptotic Complexity (Big O) for the given problem size
	state.SetComplexityN(keyCount);
}

BENCHMARK_REGISTER_F(FileStorageBenchmarkFixture, BM_store)
	->Arg(10)
	->Arg(100)
	->Arg(1000)
	->Arg(10000)
	->Arg(100000)
	->Complexity();

//--------------------------------------------------------------------------

BENCHMARK_F(FileStorageBenchmarkFixture, BM_get)(benchmark::State &state)
{
	// Setup code
	FileStorage storage{ TEST_DST_DIR "database.ini" };
	storage.load();

	const std::string key{"Key1"};

	for (auto _: state) {
		// Benchmake code
		storage.get(key);
	}
}

//--------------------------------------------------------------------------

BENCHMARK_F(FileStorageBenchmarkFixture, BM_set)(benchmark::State &state)
{
	// Setup code
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	const std::string key{"Key1"};
	const std::string value{"Value1"};

	for (auto _: state) {
		// Except code from benchmark
		state.PauseTiming();
		storage.del(key);
		state.ResumeTiming();

		// Benchmake code
		storage.set(key, value);
	}
}

//--------------------------------------------------------------------------

BENCHMARK_F(FileStorageBenchmarkFixture, BM_del)(benchmark::State &state)
{
	// Setup code
	FileStorage storage{ TEST_DST_DIR "database.ini" };

	const std::string key{"Key1"};
	const std::string value{"Value1"};

	for (auto _: state) {
		// Except code from benchmark
		state.PauseTiming();
		storage.set(key, value);
		state.ResumeTiming();

		// Benchmake code
		storage.del(key);
	}
}

//--------------------------------------------------------------------------

} // test::unit
