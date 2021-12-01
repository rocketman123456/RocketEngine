#pragma once
#include <iostream>
#include <mutex>

namespace { static inline std::mutex std_out_lock; }

template<typename... Ts>
inline void SimpleTrace(Ts&&... args) {
	std::scoped_lock lock(std_out_lock);
	(std::cout << ... << args) << std::endl;
}
