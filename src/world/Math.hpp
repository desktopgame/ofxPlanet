#pragma once
#ifndef WORLD_MATH_HPP
#define WORLD_MATH_HPP

namespace ofxPlanet {
namespace Math {
/**
 * @param f
 * @return
 */
int floatToInt(float f);


/**
 * @param f
 * @return
 */
float signum(float f);

/**
 * @param s
 * @param ds
 * @return
 */
float intbound(float s, float ds);

/**
 * @param f
 * @return
 */
bool isZero(float f, float limit = 0.0001f);
}
}
#endif