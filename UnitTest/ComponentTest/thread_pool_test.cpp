#include "MultiThread/ThreadPool.h"
#include "Utils/AnsiEscapeCode.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>

using namespace std;
using namespace std::chrono;

using namespace Rocket;
using namespace Rocket::AnsiEscapeCode;

std::string with_commas(uint64_t value) {
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

template<typename PT>
void benchmark(
		bool fast_path, 
		const char* pool_name, 
		uint64_t tasks, 
		uint64_t reps, 
		std::size_t pool_threads, 
		std::size_t push_threads) {

	cout << reset << bold << pool_name << normal << " (" << red << with_commas(tasks) << reset << " tasks, " << red << with_commas(reps) << reset << " reps)" << flush;

	std::atomic_uint check{};
	auto work = [&](uint64_t r) {
		volatile uint32_t sum = 0;
		while (r--) {
			++sum;
		}
		check.fetch_add(sum, std::memory_order_relaxed);
	};

	auto start_time = high_resolution_clock::now(); {
		PT pool{ pool_threads };

		using threads = std::vector<std::thread>;
		threads ts;

		for(int i = 0; i < push_threads; ++i) {
			ts.emplace_back([&] {
				for (uint64_t i = 1; i <= tasks; ++i) {
					if(fast_path) pool.EnqueueWork(work, reps);
					else [[maybe_unused]] auto p = pool.EnqueueTask(work, reps);
				}
			});
		}

		for(auto& t : ts)
			t.join();
	}
	auto end_time = high_resolution_clock::now();

	auto good = (check == tasks * reps * push_threads);
	cout << "\t" << red << duration_cast<microseconds>(end_time - start_time).count() / 1000.f << " ms" << "\t" << reset;
	if(good) cout << "(" << green << with_commas(check) << reset << ")";
	else cout << "(" << red << with_commas(check) << reset << ")"; wcout << blue;
	cout << endl;
}

int main() {
	auto cores = std::thread::hardware_concurrency();

	uint64_t TASK_START = 0;
	uint64_t TASK_STEP  = 250'000;
	uint64_t TASK_STOP  = 1'000'000;

	uint64_t REPS_START = 0;
	uint64_t REPS_STEP  = 25;
	uint64_t REPS_STOP  = 100;

	for(auto t = TASK_START; t <= TASK_STOP; t += TASK_STEP) {
		if(TASK_START < TASK_STOP)
			cout << "********************************************************************************" << endl;

		for(auto r = REPS_START; r <= REPS_STOP; r += REPS_STEP) {
			benchmark<SimpleThreadPool>(true, "S/fast", !t ? 100'000 : t, !r ? 1 : r, cores, cores / 2);
			benchmark<ThreadPool>       (true, "A/fast", !t ? 100'000 : t, !r ? 1 : r, cores, cores / 2);
			benchmark<SimpleThreadPool>(false, "S/slow", !t ? 100'000 : t, !r ? 1 : r, cores, cores / 2);
			benchmark<ThreadPool>       (false, "A/slow", !t ? 100'000 : t, !r ? 1 : r, cores, cores / 2);

			if(REPS_START < REPS_STOP)
				cout << endl;
		}

		if(TASK_START < TASK_STOP)
			cout << endl;
	}
}