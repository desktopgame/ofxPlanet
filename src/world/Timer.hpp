#pragma once
#ifndef WORLD_TIMER_HPP
#define WORLD_TIMER_HPP
#include <chrono>
#include <sstream>

namespace ofxPlanet {
class Timer {
       public:
        explicit Timer();

        void start();
        void end();
        std::string str() const;

       private:
        std::chrono::system_clock::time_point t0;
        std::chrono::system_clock::time_point t1;
        std::stringstream buf;
};
}  // namespace ofxPlanet
#endif