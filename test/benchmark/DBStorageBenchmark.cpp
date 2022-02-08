#include "persistence/DBStorage.hpp"

#include <stdexcept>
#include <filesystem>

#include <benchmark/benchmark.h>

namespace test::performance {

//--------------------------------------------------------------------------

class DBStorageBenchmarkFixture : public benchmark::Fixture
{
public:
	DBStorageBenchmarkFixture()
	: _value1{}
	, _value2{}
	, _value3{}
	, _value4{}
	{
	}

	void SetUp(const benchmark::State& state) override {
		DBStorage storage{ TEST_DST_DIR "database.db" };

		storage.load();

		// Save original values
		_value1 = storage.get("Key1");
		_value2 = storage.get("Key2");
		_value3 = storage.get("Key3");
		_value4 = storage.get("Key4");
	}

	void TearDown(const benchmark::State& state) override {
		DBStorage storage{ TEST_DST_DIR "database.db" };

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

BENCHMARK_DEFINE_F(DBStorageBenchmarkFixture, BM_load)(benchmark::State &state)
{	
	std::size_t keyCount = state.range(0);
	{
		// Setup code. Create N keys to store and them load. Required to calculate
		// algorithmic complexity
		DBStorage storage{ TEST_DST_DIR "database.db" };
		for (int keyIdx = 0; keyIdx < keyCount; keyIdx++) {
			std::stringstream ss;
			ss << "Key" << keyIdx;
			std::string key = ss.str();
			storage.set(key, "Value");
		}
		storage.store();
	}

	DBStorage storage{ TEST_DST_DIR "database.db" };
	for (auto _: state) {
		// Benchmake code
		storage.load();
	}

	// Asymptotic Complexity (Big O) for the given problem size
	state.SetComplexityN(keyCount);
}

BENCHMARK_REGISTER_F(DBStorageBenchmarkFixture, BM_load)
	->RangeMultiplier(10)
	->Range(10, 10000)
	->Complexity();

//--------------------------------------------------------------------------

BENCHMARK_DEFINE_F(DBStorageBenchmarkFixture, BM_store)(benchmark::State &state)
{
    DBStorage storage{ TEST_DST_DIR "database.db" };

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

BENCHMARK_REGISTER_F(DBStorageBenchmarkFixture, BM_store)
	->RangeMultiplier(10)
	->Range(10, 10000)
	->Complexity();

//--------------------------------------------------------------------------

BENCHMARK_F(DBStorageBenchmarkFixture, BM_get)(benchmark::State &state)
{
	DBStorage storage{ TEST_DST_DIR "database.db" };

	const std::string key{"Key1"};

	for (auto _: state) {
		// Benchmake code
		storage.get(key);
	}
}

//--------------------------------------------------------------------------

BENCHMARK_F(DBStorageBenchmarkFixture, BM_set)(benchmark::State &state)
{
	DBStorage storage{ TEST_DST_DIR "database.db" };

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

BENCHMARK_F(DBStorageBenchmarkFixture, BM_del)(benchmark::State &state)
{
	// Setup code
	DBStorage storage{ TEST_DST_DIR "database.db" };

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

} // test::performance
