#pragma once
#ifndef WORLD_IAREABOUNDS_HPP
#define WORLD_IAREABOUNDS_HPP
#include <memory>

namespace ofxPlanet {
class IAreaBounds : public std::enable_shared_from_this<IAreaBounds> {
       public:
        explicit IAreaBounds() = default;
        virtual ~IAreaBounds() {}
        /**
         * return true if exists block a specified position, and it block shape
         * is an `Block`.
         * @param x
         * @param y
         * @param z
         * @return
         */
        virtual bool isFilled(int x, int y, int z) const = 0;

       private:
};
}  // namespace ofxPlanet
#endif