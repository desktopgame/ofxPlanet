#pragma once
#ifndef WORLD_GRAPHICSRENDERER_HPP
#define WORLD_GRAPHICSRENDERER_HPP
#include <ofShader.h>

#include <memory>
#include <unordered_map>

namespace planet {
class World;
class GraphicsBatch;
class GraphicsRenderer {
       public:
        GraphicsRenderer(const World& world, ofShader& shader);

        void putFront(GLuint texture, int x, int y, int z);
        void putBack(GLuint texture, int x, int y, int z);
        void putLeft(GLuint texture, int x, int y, int z);
        void putRight(GLuint texture, int x, int y, int z);
        void putTop(GLuint texture, int x, int y, int z);
        void putBottom(GLuint texture, int x, int y, int z);
        void clear();
        void update();
        void render();

       protected:
        virtual std::shared_ptr<GraphicsBatch> createBatch(
            const World& world, ofShader& shader) = 0;

       private:
        const World& world;
        ofShader& shader;
        std::unordered_map<GLuint, std::shared_ptr<GraphicsBatch> > map;
        std::shared_ptr<GraphicsBatch> ref(GLuint texture);
};
}  // namespace planet
#endif