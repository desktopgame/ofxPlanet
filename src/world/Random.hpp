#pragma once
#ifndef COMMON_RANDOM_HPP
#define COMMON_RANDOM_HPP
#include <random>
namespace ofxPlanet {
/**
 * Randomは標準APIのラッパーです。
 */
class Random {
       public:
        explicit Random();

        /**
         * 指定の範囲の乱数を生成します。
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