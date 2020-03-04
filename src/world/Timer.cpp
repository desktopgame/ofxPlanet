#include "Timer.hpp"
namespace ofxPlanet {
Timer::Timer() : t0(), t1(), buf() {
}
void Timer::start() {
	buf.str("");
	buf.clear(std::stringstream::goodbit);
	this->t0 = std::chrono::system_clock::now();
}
void Timer::end() {
	this->t1 = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	buf << elapsed << "ms";
}
std::string Timer::str() const {
	return buf.str();
}
}