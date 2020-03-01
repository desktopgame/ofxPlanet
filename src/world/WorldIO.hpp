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

namespace ofxPlanet {
class FixedWorld;
class Planet;
/**
 * Progress is progress for background generation process.
 */
class Progress {
       public:
        explicit Progress();
        /**
         * set progress.
         * @param value
         */
        void setValue(float value);
        /**
         * returns a progress.
         * @return
         */
        float getValue() const;
        /**
         * return true if progress finished.
         * @return
         */
        bool isDone() const;

       private:
        float value;
};
using ReadonlyProgress = const Progress;
using AsyncOperation = std::shared_ptr<ReadonlyProgress>;
/**
 * WorldIO is utility class for terrain export.
 */
class WorldIO {
       public:
        /**
         * generate as Json.
         * @param outputFile
         * @param world
         * @return
         */
        static AsyncOperation saveJson(const std::string& outputFile,
                                       const std::shared_ptr<FixedWorld>& world);
        /**
         * generate as Obj.
         * @param outputDir
         * @param world
         * @return
         */
        static AsyncOperation saveObj(const std::string& outputDir,
                                      const std::shared_ptr<FixedWorld>& world);
        /**
         * generate as Obj.
         * @param outputDir
         * @param world
         * @return
         */
        static AsyncOperation saveObj(const std::string& outputDir,
                                      const std::shared_ptr<FixedWorld>& world,
                                      int splitCount);
        /**
         * generate as Bmp.
         * @param outputFile
         * @param planet
         * @return
         */
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
                                 const std::shared_ptr<FixedWorld>& world);

        static void genTopPlane(const std::string& outputDir,
                                std::vector<std::string>& texVec,
                                objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                glm::ivec3 worldPos, glm::ivec3 objPos,
                                glm::vec3 size, glm::vec3 offset,
                                const std::shared_ptr<FixedWorld>& world);
        static void genBottomPlane(const std::string& outputDir,
                                   std::vector<std::string>& texVec,
                                   objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                   glm::ivec3 worldPos, glm::ivec3 objPos,
                                   glm::vec3 size, glm::vec3 offset,
                                   const std::shared_ptr<FixedWorld>& world);
        static void genLeftPlane(const std::string& outputDir,
                                 std::vector<std::string>& texVec,
                                 objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                 glm::ivec3 worldPos, glm::ivec3 objPos,
                                 glm::vec3 size, glm::vec3 offset,
                                 const std::shared_ptr<FixedWorld>& world);
        static void genRightPlane(const std::string& outputDir,
                                  std::vector<std::string>& texVec,
                                  objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                  glm::ivec3 worldPos, glm::ivec3 objPos,
                                  glm::vec3 size, glm::vec3 offset,
                                  const std::shared_ptr<FixedWorld>& world);
        static void genFrontPlane(const std::string& outputDir,
                                  std::vector<std::string>& texVec,
                                  objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                  glm::ivec3 worldPos, glm::ivec3 objPos,
                                  glm::vec3 size, glm::vec3 offset,
                                  const std::shared_ptr<FixedWorld>& world);
        static void genBackPlane(const std::string& outputDir,
                                 std::vector<std::string>& texVec,
                                 objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                                 glm::ivec3 worldPos, glm::ivec3 objPos,
                                 glm::vec3 size, glm::vec3 offset,
                                 const std::shared_ptr<FixedWorld>& world);

        WorldIO() = delete;
        ~WorldIO() = delete;
};
}  // namespace ofxPlanet
#endif