#pragma once
#ifndef WORLD_WORLDIO_HPP
#define WORLD_WORLDIO_HPP
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string>

#include "objb/MtlBuilder.hpp"
#include "objb/ObjBuilder.hpp"

namespace planet {
class World;
class Planet;

class Progress {
       public:
        explicit Progress();

        void setValue(float value);
        float getValue() const;
        bool isDone() const;

       private:
        float value;
};
using ReadonlyProgress = const Progress;
using AsyncOperation = std::shared_ptr<ReadonlyProgress>;

class WorldIO {
       public:
        static AsyncOperation saveJson(const std::string& outputFile,
                                       const std::shared_ptr<World>& world);
        static AsyncOperation saveObj(const std::string& outputDir,
                                      const std::shared_ptr<World>& world);
        static AsyncOperation saveObj(const std::string& outputDir,
                                      const std::shared_ptr<World>& world,
                                      int splitCount);
        static AsyncOperation saveBmp(const std::string& outputFile,
                                      const std::shared_ptr<Planet>& planet);

       private:
        template <typename T>
        static float onef(T a) {
                return std::max(1.0f, static_cast<float>(a));
        }

        template <typename T>
        static float sumf(T a) {
                return static_cast<float>(onef(a));
        }

        template <typename T>
        static float sumf(T a, T b) {
                return static_cast<float>(onef(a) * onef(b));
        }

        template <typename T>
        static float sumf(T a, T b, T c) {
                return static_cast<float>(onef(a) * onef(b) * onef(c));
        }

        static glm::vec3 asVec3(int x, int y, int z);

        static void saveObjAsync(std::shared_ptr<Progress> progress,
                                 const std::string& outputDir,
                                 const std::shared_ptr<World>& world);

        static void genTopPlane(const std::string& outputDir,
                                std::vector<std::string>& texVec,
                                objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                glm::ivec3 worldPos, glm::ivec3 objPos,
                                glm::vec3 size, glm::vec3 offset,
                                const std::shared_ptr<World>& world);
        static void genBottomPlane(const std::string& outputDir,
                                   std::vector<std::string>& texVec,
                                   objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                   glm::ivec3 worldPos, glm::ivec3 objPos,
                                   glm::vec3 size, glm::vec3 offset,
                                   const std::shared_ptr<World>& world);
        static void genLeftPlane(const std::string& outputDir,
                                 std::vector<std::string>& texVec,
                                 objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                 glm::ivec3 worldPos, glm::ivec3 objPos,
                                 glm::vec3 size, glm::vec3 offset,
                                 const std::shared_ptr<World>& world);
        static void genRightPlane(const std::string& outputDir,
                                  std::vector<std::string>& texVec,
                                  objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                  glm::ivec3 worldPos, glm::ivec3 objPos,
                                  glm::vec3 size, glm::vec3 offset,
                                  const std::shared_ptr<World>& world);
        static void genFrontPlane(const std::string& outputDir,
                                  std::vector<std::string>& texVec,
                                  objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                  glm::ivec3 worldPos, glm::ivec3 objPos,
                                  glm::vec3 size, glm::vec3 offset,
                                  const std::shared_ptr<World>& world);
        static void genBackPlane(const std::string& outputDir,
                                 std::vector<std::string>& texVec,
                                 objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                 glm::ivec3 worldPos, glm::ivec3 objPos,
                                 glm::vec3 size, glm::vec3 offset,
                                 const std::shared_ptr<World>& world);

        WorldIO() = delete;
        ~WorldIO() = delete;
};
}  // namespace planet
#endif