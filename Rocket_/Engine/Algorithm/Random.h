#pragma once
#include <random>
#include <climits>

namespace Rocket {
    namespace Algorithm {
        class Random {
        public:
            void SetSeed(long s) { e.seed(s); }
            int UniformInt(int N) {
                if (N <= 0)
                    throw std::invalid_argument("Parameter N must be positive");
                std::uniform_int_distribution<unsigned> u(0, N);
                return u(e);
            }

            int UniformInt(int lo, int hi) {
                if (hi < lo || (long)(hi - lo)>INT_MAX)
                    throw std::invalid_argument("range error");
                std::uniform_int_distribution<int> u(lo, hi);
                return u(e);
            }

            double Uniform() {
                std::uniform_real_distribution<double> u(0, 1);
                return u(e);
            }

            double Uniform(double lo, double hi) {
                if (lo >= hi)
                    throw std::invalid_argument("range error");
                std::uniform_real_distribution<double>u(lo, hi);
                return u(e);
            }

            bool Bernoulli(double p = 0.5) {
                if (p <= 0 || p >= 1)
                    throw std::invalid_argument("Probability must be between 0.0 and 1.0");
                std::bernoulli_distribution b(p);
                return b(e);
            }

            double Gaussian() {
                std::normal_distribution<double> n;
                return n(e);
            }

            double Gaussian(double m, double s) {
                std::normal_distribution<double> n(m, s);
                return n(e);
            }

            template<typename It>
            int Discrete(It beg, It end) {
                double r = Uniform();
                double sum = 0;
                for (auto it = beg; it != end; ++it) {
                    sum += *it;
                    if (sum >= r)return (it - beg);
                }
                return -1;
            }

            template<typename It>
            void Shuffle(It beg, It end) {
                int N = (end - beg);
                for (int i = 0; i < N; ++i) {
                    int r = UniformInt(i, N - 1);
                    std::swap(*(beg + i), *(beg + r));
                }
            }
        private:
            std::default_random_engine e;
        };
    } // namespace Math
}
