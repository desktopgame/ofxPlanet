#pragma once
#ifndef WORLD_GRAPHICSBATCH_HPP
#define WORLD_GRAPHICSBATCH_HPP
#include <ofShader.h>

#include <glm/glm.hpp>

namespace ofxPlanet {
class World;
/**
 * GraphicsBatch is store position and vertex information.
 */
class GraphicsBatch {
       public:
        GraphicsBatch(const World& world, ofShader& shader);
        virtual ~GraphicsBatch() {}
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putFront(int x, int y, int z) = 0;
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putBack(int x, int y, int z) = 0;
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putLeft(int x, int y, int z) = 0;
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putRight(int x, int y, int z) = 0;
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putTop(int x, int y, int z) = 0;
        /**
         * add position for specific side.
         * @param x
         * @param y
         * @param z
         */
        virtual void putBottom(int x, int y, int z) = 0;
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
        glm::vec3 getPhysicalPosition(int x, int y, int z) const;
        const World& world;
        ofShader& shader;
};
}  // namespace ofxPlanet
#endif