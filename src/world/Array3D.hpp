#ifndef WORLD_ARRAY3D_HPP
#define WORLD_ARRAY3D_HPP
#include <vector>

namespace ofxPlanet {
/**
 * Array3D is three dimensional data container.
 */
template<typename T>
class Array3D {
public:
	using Line = std::vector<T>;
	using Table = std::vector<Line>;
	using Container = std::vector<Table>;
    explicit Array3D(int xSize, int ySize, int zSize, T defautValue)
     : xSize(xSize), ySize(ySize), zSize(zSize), vec() {
        for (int x = 0; x < xSize; x++) {
                std::vector<std::vector<T>> xline;
                for (int y = 0; y < ySize; y++) {
                        std::vector<T> yline;
                        for (int z = 0; z < zSize; z++) {
                                yline.emplace_back(defautValue);
                        }
                        xline.emplace_back(yline);
                }
                vec.emplace_back(xline);
        }
    }
    explicit Array3D(int xSize, int ySize, int zSize) : Array3D(xSize, ySize, zSize, T()) {
    }
	virtual ~Array3D(){}

	/**
	 * @param x
	 * @param y
	 * @param z
	 * @param val
	 */
    void set(int x, int y, int z, const T& val) {
        vec[x][y][z] = val;
    }

	/**
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
    T get(int x, int y, int z) const {
        return vec[x][y][z];
    }

	/**
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	bool contains(int x, int y, int z) const {
		return x >= 0 && y >= 0 && z >= 0 && x < xSize && y < ySize && z < zSize;
	}

	/**
	 * @return
	 */
	int getXSize() const {
		return xSize;
	}

	/**
	 * @return
	 */
	int getYSize() const {
		return ySize;
	}

	/**
	 * @return
	 */
	int getZSize() const {
		return zSize;
	}

	/**
	 * @return
	 */
	Container::iterator begin() {
		return vec.begin();
	}

	/**
	 * @return
	 */
	Container::iterator end() {
		return vec.end();
	}

	/**
	 * @return
	 */
	Container::const_iterator cbegin() {
		return vec.cbegin();
	}

	/**
	 * @return
	 */
	Container::const_iterator cend() {
		return vec.cend();
	}

	/** 
	 * @return
	 */
	Container::iterator rbegin() {
		return vec.rbegin();
	}

	/**
	 * @return
	 */
	Container::iterator rend() {
		return vec.rend();
	}

	/**
	 * @return
	 */
	Container::const_iterator crbegin() {
		return vec.crbegin();
	}

	/**
	 * @return
	 */
	Container::const_iterator crend() {
		return vec.crend();
	}
protected:
    int xSize;
    int ySize;
    int zSize;
    Container vec;
};
}
#endif