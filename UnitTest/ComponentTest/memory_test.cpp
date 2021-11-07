#define RK_MEMORY_CHECK
#include "Memory/MemoryCheck.h"
#include "Memory/MemoryCheck.cpp"

#ifndef new
#define new new(__FILE__, __LINE__, __FUNCTION__)
#endif

template<typename T, int N> void good() {
	delete (new T);
	delete [] (new T [N]);

	struct Boom { Boom() { throw 1; } };

	try { new Boom; } catch(...) { } // not a leak!
	try { new Boom [N]; } catch(...) { } // same!
}

template<typename T, int N> void mismatch() {
	delete [] (new T);
	delete (new T [N]);
}

template<typename T, int N> void leak() {
	/*delete*/ new T;
	/*delete []*/ new T [N];
}

int main() {
    good<char, 100>();
	mismatch<char, 100>();
	leak<char, 100>();

    return 0;
}
