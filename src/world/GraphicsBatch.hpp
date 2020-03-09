#pragma once
#ifndef WORLD_GRAPHICSBATCH_HPP
#define WORLD_GRAPHICSBATCH_HPP
#include <ofShader.h>

#include <glm/glm.hpp>
#include "GraphicsRenderer.hpp"

namespace ofxPlanet {
class World;
/**
 * GraphicsBatch is store position and vertex information.
 */
class GraphicsBatch {
       public:
        GraphicsBatch(ofShader& shader);
        virtual ~GraphicsBatch() {}
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putFront(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putBack(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putLeft(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putRight(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putTop(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * add position for specific side.
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        virtual void putBottom(int brightness, PositionType x, PositionType y, PositionType z) = 0;
        /**
         * clear all vertex.
         */
        virtual void clear() = 0;
        /**
         * update all primitive.
         */
        virtual void update() = 0;
        /**
         * draw all primitive.
         * @param texture
         */
        virtual void render(GLuint texture) = 0;

       protected:
        ofShader& shader;
};
}  // namespace ofxPlanet
#endif