#pragma once
#include <ostream>

namespace Rocket {
    class MilliSecond {
    public:
        MilliSecond(double time) : time_(time) {}
        operator double() const { return time_; }
        //inline double Get() { return time_; }
    private:
        double time_;
    };

    class Second {
    public:
        Second(double time) : time_(time) {}
        operator double() const { return time_; }
        //inline double Get() { return time_; }
    private:
        double time_;
    };

    class TimeStep {
    public:
        TimeStep(double milli = 0.0f) : time_(milli) {}
        TimeStep(const MilliSecond& milli_second) : time_(milli_second) {}
        TimeStep(const Second& second) : time_(second * 1000.0) {}

        operator float() const { return static_cast<float>(time_); }
        operator double() const { return time_; }

        inline double GetSeconds() const { return time_ * 0.001; }
        inline double GetMilliseconds() const { return time_; }
    private:
        double time_;
    };

    inline std::ostream& operator << (std::ostream& os, const MilliSecond& s) {
		return os << s << " ms ";
	}

    inline std::ostream& operator << (std::ostream& os, const Second& s) {
		return os << s << " s ";
	}

    inline std::ostream& operator << (std::ostream& os, const TimeStep& s) {
		return os << s.GetMilliseconds() << " ms dt ";
	}
}