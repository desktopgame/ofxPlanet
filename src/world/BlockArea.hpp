#ifndef WORLD_BLOCKAREA_HPP
#define WORLD_BLOCKAREA_HPP
#include <glm/glm.hpp>
#include <vector>

namespace ofxPlanet {
/**
 * BlockArea is point list for same height and connected on X or Z direction.
 */
class BlockArea {
public:
	explicit BlockArea();

	/**
	 * add point.
	 * @param point
	 */
	void addPoint(glm::ivec3 point);
	/**
	 * returns point from index.
	 * @param i
	 * @return
	 */
	glm::ivec3 getPoint(int i) const;
	/**
	 * returns count of points.
	 * @return
	 */
	int getPointCount() const;
	/**
	 * returns points.
	 * @return
	 */
	std::vector<glm::ivec3> getPoints() const;
	/**
	 * compute a 2D size.
	 */
	glm::ivec3 compute2DSize() const;

private:
	std::vector<glm::ivec3> points;
};
}
#endif