#pragma once
#include <assert.h>
#include <stdio.h>

namespace Rocket {
	class FPSCounter
	{
	public:
		explicit FPSCounter(float avgInterval = 0.5f) :avg_interval_(avgInterval) {
			assert(avgInterval > 0.0f);
		}
		bool Tick(float deltaSeconds, bool frameRendered = true) {
			if (frameRendered)
				num_frames_++;
			accumulatedTime_ += deltaSeconds;
			if (accumulated_time_ > avgInterval_) {
				currentFPS_ = static_cast<float>(num_frames_ / accumulated_time_);
				if (printFPS_)
					printf("FPS: %.1f\n", currentFPS_);
				num_frames_ = 0;
				accumulated_time_ = 0;
				return true;
			}
			return false;
		}
		inline float getFPS() const { 
			return currentFPS_; 
		}
	private:
		const float avg_interval_ = 0.5f;
		unsigned int num_frames_ = 0;
		double accumulated_time_ = 0;
		float currentFPS_ = 0.0f;
		bool printFPS_ = true;
	};
}
