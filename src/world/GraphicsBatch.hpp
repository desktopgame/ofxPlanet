#pragma once
#ifndef WORLD_GRAPHICSBATCH_HPP
#define WORLD_GRAPHICSBATCH_HPP
#include <ofShader.h>

namespace planet {
class World;
class GraphicsBatch {
       public:
        GraphicsBatch(const World& world, ofShader& shader);
        virtual ~GraphicsBatch() {}

        virtual void putFront(int x, int y, int z) = 0;
        virtual void putBack(int x, int y, int z) = 0;
        virtual void putLeft(int x, int y, int z) = 0;
        virtual void putRight(int x, int y, int z) = 0;
        virtual void putTop(int x, int y, int z) = 0;
        virtual void putBottom(int x, int y, int z) = 0;
        virtual void clear() = 0;
        virtual void update() = 0;
        virtual void render(GLuint texture) = 0;

       protected:
        glm::vec3 getPhysicalPosition(int x, int y, int z) const;
        const World& world;
        ofShader& shader;
};
}  // namespace planet
#endif