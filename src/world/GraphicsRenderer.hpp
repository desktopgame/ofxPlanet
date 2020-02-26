#pragma once
#ifndef WORLD_GRAPHICSRENDERER_HPP
#define WORLD_GRAPHICSRENDERER_HPP
#include <ofShader.h>

#include <memory>
#include <unordered_map>

namespace ofxPlanet {
class World;
class GraphicsBatch;
/**
 * GraphicsRenderer is wrapper for GraphicsBatch.
 */
class GraphicsRenderer {
       public:
        GraphicsRenderer(ofShader& shader);
        virtual ~GraphicsRenderer() {}

        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putFront(GLuint texture, int brightness, int x, int y, int z);
        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putBack(GLuint texture, int brightness, int x, int y, int z);
        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putLeft(GLuint texture, int brightness, int x, int y, int z);
        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putRight(GLuint texture, int brightness, int x, int y, int z);
        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putTop(GLuint texture, int brightness, int x, int y, int z);
        /**
         * add position to GraphicsBatch.
         * @param texture
         * @param brightness
         * @param x
         * @param y
         * @param z
         */
        void putBottom(GLuint texture, int brightness, int x, int y, int z);

        /**
         * clear all vertex.
         */
        void clear();
        /**
         * update all primitive.
         */
        void update();
        /**
         * draw all GraphicsBatch.
         */
        void render();

       protected:
        virtual std::shared_ptr<GraphicsBatch> createBatch(ofShader& shader) = 0;

       private:
        ofShader& shader;
        std::unordered_map<GLuint, std::shared_ptr<GraphicsBatch> > map;
        std::shared_ptr<GraphicsBatch> ref(GLuint texture);
};
}  // namespace ofxPlanet
#endif