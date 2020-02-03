#include "Planet.hpp"

#include "World.hpp"
#include "biome/Biome.hpp"
namespace planet {

Planet::Planet(ofShader& shader)
    : shader(shader), world(nullptr), blockTable(0, 0, 0) {}

void Planet::generate(glm::vec3 size, std::shared_ptr<Biome> biome) {
        this->world = World::create(shader, size);
        glm::ivec3 is = size;
        this->blockTable = BlockTable(is.x, is.y, is.z);
        biome->generate(blockTable);
        world->load(blockTable);
}

void Planet::update() {
        if (world) {
                world->update();
        }
}

void Planet::drawToBuffer() {
        if (world) {
                world->drawToBuffer();
        }
}

void Planet::render() {
        if (world) {
                world->render();
        }
}

std::shared_ptr<World> Planet::getWorld() const { return world; }
BlockTable Planet::getBlockTable() const { return blockTable; }
}  // namespace planet
