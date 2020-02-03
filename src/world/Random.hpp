#pragma once
#ifndef COMMON_RANDOM_HPP
#define COMMON_RANDOM_HPP
#include <random>
namespace ofxPlanet {
/**
 * Random is wrapper of standard random library.
 */
class Random {
       public:
        explicit Random();

        /**
         * generate random number in range.
         * @param min
         * @param max
         * @retunr
         */
        template <typename T>
        T generate(const T min, const T max);

       private:
        std::random_device device;
        std::mt19937 algo;
};

template <typename T>
inline T Random::generate(const T min, const T max) {
        std::uniform_real_distribution<T> gen(min, max);
        return gen(algo);
}

template <>
inline int Random::generate(const int min, const int max) {
        std::uniform_int_distribution<int> gen(min, max);
        return gen(algo);
}

template <>
inline float Random::generate(const float min, const float max) {
        std::uniform_real_distribution<float> gen(min, max);
        return gen(algo);
}

template <>
inline double Random::generate(const double min, const double max) {
        std::uniform_real_distribution<double> gen(min, max);
        return gen(algo);
}
}  // namespace ofxPlanet
#endif