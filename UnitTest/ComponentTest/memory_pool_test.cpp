#include "Memory/MemoryCheck.h"
#include "Memory/MemoryPool.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

// #include "Memory/MemoryAllocator.h"

using namespace Rocket;

int main() {
	using namespace std;

	// MyAllocator<double> alloc;

	constexpr auto I = 3;
	constexpr auto BLOCKS = 3;
	constexpr auto CHUNK_SIZE = 10;
	constexpr auto CHUNKS_PER_BLOCK = 10;
	constexpr auto CHUNKS_TO_MALLOC = CHUNKS_PER_BLOCK * BLOCKS;
	constexpr auto BLOCK_RESERVE = 1;

	using chunks_t = std::vector<void*>;

	auto chunks = chunks_t{};
	auto gen = std::mt19937{ (std::random_device{})() };

	auto pool = Rocket::MemoryPool<CHUNK_SIZE, CHUNKS_PER_BLOCK>{};
	pool.reserve_blocks(BLOCK_RESERVE);

	cout << pool << endl << endl;

	for(auto i = 0; i < I; ++i) {
		for(auto c = 0; c < CHUNKS_TO_MALLOC - i; ++c)
			chunks.push_back(pool.malloc());
		cout << pool << endl << endl;

		std::shuffle(std::begin(chunks), std::end(chunks), gen);

		for(auto chunk : chunks)
			pool.free(chunk);
		cout << pool << endl << endl;

		chunks.clear();
	}
}
