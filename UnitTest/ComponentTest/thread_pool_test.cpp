#include "MultiThread/ThreadPool.h"

#include <iostream>

int main()
{
	using namespace std;
	srand(time(NULL));
	mutex cout_guard;

	cout << "main thread ID: " << this_thread::get_id() << endl;

	Rocket::ThreadPool tp;

	for(auto i = 1; i <= 50; i++) {
		tp.DoWork([&, i = i]() {
			{
				unique_lock guard(cout_guard);
				cout << "doing work " << i << "..." << endl;
			}
			this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
		});
	}
}