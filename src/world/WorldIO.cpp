#include "WorldIO.hpp"

#include <ofFileUtils.h>
#include <ofxSOIL.h>

#include <future>
#include <sstream>
#include <thread>

#include "picojson/picojson.h"
#include "Block.hpp"
#include "BlockPack.hpp"
#include "Planet.hpp"
#include "TexturePack.hpp"
#include "World.hpp"

namespace planet {
// Progress
Progress::Progress() : value(0.0f) {}
void Progress::setValue(float value) {
        if (value < this->value) {
                value = this->value;
        }
        this->value = value;
}
float Progress::getValue() const { return this->value; }
bool Progress::isDone() const { return this->value >= 1.0f; }
// WorldIO
AsyncOperation WorldIO::saveJson(const std::string& outputFile,
                                 const std::shared_ptr<World>& world) {
        auto ret = std::make_shared<Progress>();
        if (!world) {
                ret->setValue(1.0f);
                return ret;
        }
        std::thread([outputFile, world, ret]() -> void {
                auto outputPath = ofFilePath::join(
                    ofFilePath::getCurrentExeDir(), outputFile);
                ofFile::removeFile(outputPath);
                picojson::object rootO;
                picojson::array blocksA;
                picojson::object worldSizeO;
                std::stringstream ss;
                int xs = world->getXSize();
                int ys = world->getYSize();
                int zs = world->getZSize();
                float all = static_cast<float>(xs * ys * zs);
                for (int x = 0; x < xs; x++) {
                        for (int y = 0; y < ys; y++) {
                                for (int z = 0; z < zs; z++) {
                                        bool overXP = x + 1 >= xs;
                                        bool overXN = x - 1 < 0;
                                        bool overYP = y + 1 >= ys;
                                        bool overYN = y - 1 < 0;
                                        bool overZP = z + 1 >= zs;
                                        bool overZN = z - 1 < 0;

                                        std::shared_ptr<Block> null = nullptr;
                                        auto blockXP =
                                            overXP
                                                ? null
                                                : world->getBlock(x + 1, y, z);
                                        auto blockXN =
                                            overXN
                                                ? null
                                                : world->getBlock(x - 1, y, z);
                                        auto blockYP =
                                            overYP
                                                ? null
                                                : world->getBlock(x, y + 1, z);
                                        auto blockYN =
                                            overYN
                                                ? null
                                                : world->getBlock(x, y - 1, z);
                                        auto blockZP =
                                            overZP
                                                ? null
                                                : world->getBlock(x, y, z + 1);
                                        auto blockZN =
                                            overZN
                                                ? null
                                                : world->getBlock(x, y, z - 1);
                                        auto blockB = world->getBlock(x, y, z);
                                        if (blockB == nullptr) {
                                                ret->setValue(sumf(x, y, z) /
                                                              all);
                                                continue;
                                        }
                                        auto block = blockB;
                                        if (blockXP && blockXN && blockYP &&
                                            blockYN && blockZP && blockZN) {
                                        } else {
                                                picojson::object blockO;
                                                blockO["x"] = picojson::value(
                                                    static_cast<double>(x));
                                                blockO["y"] = picojson::value(
                                                    static_cast<double>(y));
                                                blockO["z"] = picojson::value(
                                                    static_cast<double>(z));
                                                blockO["block"] =
                                                    picojson::value(
                                                        block->getName());
                                                blocksA.emplace_back(blockO);
                                        }
                                        ret->setValue(sumf(x, y, z) / all);
                                }
                        }
                }
                worldSizeO["x"] =
                    picojson::value(static_cast<double>(world->getXSize()));
                worldSizeO["y"] =
                    picojson::value(static_cast<double>(world->getYSize()));
                worldSizeO["z"] =
                    picojson::value(static_cast<double>(world->getZSize()));
                rootO["cell"] = picojson::value(blocksA);
                rootO["worldSize"] = picojson::value(worldSizeO);
                ss << picojson::value(rootO) << std::endl;
                std::ofstream ofs(outputPath);
                ofs << ss.str();
                ofs.close();
                ret->setValue(1.0f);
        }).detach();
        return ret;
}

AsyncOperation WorldIO::saveObj(const std::string& outputDir,
                                const std::shared_ptr<World>& world) {
        auto ret = std::make_shared<Progress>();
        auto w = world;
        std::thread(std::bind(&WorldIO::saveObjAsync, ret, outputDir, world))
            .detach();
        return ret;
}

AsyncOperation WorldIO::saveObj(const std::string& outputDir,
                                const std::shared_ptr<World>& world,
                                int splitCount) {
        // ファイル名を作るためのバッファ初期化
        char buf[64];
        std::memset(buf, '\0', 64);
        std::string cpOutputDir = outputDir;
        // ワールドを指定の分割数で分割
        auto worlds = world->split(splitCount);
        auto asyncVec = std::vector<AsyncOperation>();
        int splitCountN = static_cast<int>(worlds.size());
        // 分割されたワールド全てに
        for (int i = 0; i < splitCountN; i++) {
                auto wpart = worlds.at(i);
                // 分割されたワールドが、どれだけのオフセットを加えれば
                // ただしい位置に表示されるかをファイル名に含める
                std::sprintf(buf, "_Split_x%dz%d", wpart.offset.x,
                             wpart.offset.z);
                // ワールドを格納するためのディレクトリを作成
                auto newOutputDir = cpOutputDir + std::string(buf);
                ofDirectory::createDirectory(ofFilePath::join(
                    ofFilePath::getCurrentExeDir(), newOutputDir));
                // 既存のファイルを削除して、生成を開始
                auto outputFile = ofFilePath::join(
                    ofFilePath::getCurrentExeDir(),
                    ofFilePath::join(newOutputDir, "data.obj"));
                asyncVec.emplace_back(
                    WorldIO::saveObj(newOutputDir, wpart.world));
        }
        // 全てのワールド生成が終わるまで待機する
        auto aAsync = std::make_shared<Progress>();
        std::thread([aAsync, asyncVec, splitCountN]() -> void {
                bool run = true;
                int count = 0;
                while (run) {
                        run = false;
                        count = 0;
                        // すべて終わるまで待機
                        for (auto aa : asyncVec) {
                                if (!aa->isDone()) {
                                        run = true;
                                } else {
                                        count++;
                                }
                        }
                        aAsync->setValue(static_cast<float>(count) /
                                         static_cast<float>(splitCountN));
                }
                aAsync->setValue(1.0f);
        }).detach();
        return aAsync;
}

AsyncOperation WorldIO::saveBmp(const std::string& outputFile,
                                const std::shared_ptr<Planet>& planet) {
        auto ret = std::make_shared<Progress>();
        auto w = planet->getWorld();
        std::thread([outputFile, planet, w, ret]() -> void {
                auto outputPath = ofFilePath::join(
                    ofFilePath::getCurrentExeDir(), outputFile);
                ofFile::removeFile(outputPath);
                BlockTable blockTable = planet->getBlockTable();
                Terrain terrain = blockTable.getTerrain();
                std::vector<unsigned char> pixelVec = terrain.toPixelVec();
                ofxSOIL::saveImage(outputFile, ofxSOIL::TYPE_BMP, w->getXSize(),
                                   w->getZSize(), 4, pixelVec.data());
                ret->setValue(1.0f);
        }).detach();
        return ret;
}

glm::vec3 WorldIO::asVec3(int x, int y, int z) { return glm::vec3(x, y, z); }

void WorldIO::saveObjAsync(std::shared_ptr<Progress> progress,
                           const std::string& outputDir,
                           const std::shared_ptr<World>& world) {
        using namespace objb;
        // 先にファイルを消しておく
        auto outputPath =
            ofFilePath::join(ofFilePath::getCurrentExeDir(),
                             ofFilePath::join(outputDir, "data.obj"));
        ofFile::removeFile(outputPath);
        ofFile::removeFile(outputPath + ".mtl");
        // 共有される頂点情報を書き込んでおく
        ObjBuilder ob;
        MtlBuilder mb;
        ob.globalTexcoord(glm::vec2(0, 0))
            .globalTexcoord(glm::vec2(1, 0))
            .globalTexcoord(glm::vec2(1, 1))
            .globalTexcoord(glm::vec2(0, 1))

			.globalTexcoord(glm::vec2(0, 1))
			.globalTexcoord(glm::vec2(0, 0))
			.globalTexcoord(glm::vec2(1, 1))
			.globalTexcoord(glm::vec2(1, 0))

            .globalNormal(glm::vec3(0, 1, 0))
            .globalNormal(glm::vec3(0, 1, 0))
            .globalNormal(glm::vec3(-1, 0, 0))
            .globalNormal(glm::vec3(0, 1, 0))
            .globalNormal(glm::vec3(0, 1, 0))
            .globalNormal(glm::vec3(0, 0, 1));
        ob.material("data.obj.mtl");
        int xsize = world->getXSize();
        int ysize = world->getYSize();
        int zsize = world->getZSize();
        float all = static_cast<float>(xsize * ysize * zsize);
        // glm::vec3 size(1, 1, 1);
        ob.reserveModels(xsize * ysize * zsize);
        std::vector<std::string> texVec;
        for (int x = 0; x < xsize; x++) {
                for (int y = 0; y < ysize; y++) {
                        for (int z = 0; z < zsize; z++) {
                                auto block = world->getBlock(x, y, z);
                                if (!block) {
                                        progress->setValue(sumf(x, y, z) / all);
                                        continue;
                                }
                                bool overXP = x + 1 >= xsize;
                                bool overYP = y + 1 >= ysize;
                                bool overZP = z + 1 >= zsize;
                                bool overXN = x - 1 < 0;
                                bool overYN = y - 1 < 0;
                                bool overZN = z - 1 < 0;

                                bool hasBlockXP = world->isFilled(x + 1, y, z);
                                bool hasBlockYP = world->isFilled(x, y + 1, z);
                                bool hasBlockZP = world->isFilled(x, y, z + 1);
                                bool hasBlockXN = world->isFilled(x - 1, y, z);
                                bool hasBlockYN = world->isFilled(x, y - 1, z);
                                bool hasBlockZN = world->isFilled(x, y, z - 1);
                                bool hiddenBlock = hasBlockXP && hasBlockYP &&
                                                   hasBlockZP && hasBlockXN &&
                                                   hasBlockYN && hasBlockZN;
                                if (hiddenBlock) {
                                        progress->setValue(sumf(x, y, z) / all);
                                        continue;
                                }
                                BlockShape shape = block->getShape();
                                glm::vec3 blockSize =
                                    sizeFromShape(BlockShape::Block);
                                glm::vec3 size = block->getSize() / 2.0f;
                                glm::vec3 offset =
                                    (blockSize - block->getSize()) / 2.0f;
                                if (shape == BlockShape::BottomSlab) {
                                        offset *= -1.0f;
                                }
                                if (shape == BlockShape::FrontSlab) {
                                        offset *= -1.0f;
                                }
                                if (shape == BlockShape::LeftSlab) {
                                        offset *= -1.0f;
                                }
                                if (!hasBlockYP) {
                                        genTopPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                if (!hasBlockYN) {
                                        genBottomPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                if (!hasBlockXP) {
                                        genRightPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                if (!hasBlockXN) {
                                        genLeftPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                if (!hasBlockZP) {
                                        genFrontPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                if (!hasBlockZN) {
                                        genBackPlane(
                                            outputDir, texVec, ob, mb,
                                            glm::ivec3(x, y, z),
                                            glm::ivec3(x * 2, y * 2, z * 2),
                                            size, offset, world);
                                }
                                progress->setValue(sumf(x, y, z) / all);
                        }
                }
        }
        // オブジェクトファイルを生成
        std::ofstream objOFS(outputPath);
        if (!objOFS.fail()) {
                ob.write(objOFS);
        }
        objOFS.close();
        // マテリアルファイルを生成
        std::ofstream mtlOFS(outputPath + ".mtl");
        if (!mtlOFS.fail()) {
                mtlOFS << mb.toString() << std::endl;
        }
        mtlOFS.close();
        progress->setValue(1.0f);
}

// private
void WorldIO::genTopPlane(const std::string& outputDir,
                          std::vector<std::string>& texVec,
                          objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                          glm::ivec3 worldPos, glm::ivec3 objPos,
                          glm::vec3 size, glm::vec3 offset,
                          const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Top", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getTopImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Top");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getTopImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global),
                         ObjIndex(1, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global),
                         ObjIndex(1, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(4, IndexMode::Global),
                         ObjIndex(1, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(3, IndexMode::Global),
                         ObjIndex(1, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(-size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(-size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);
        face.emplace_back(polyD);

        aa.face(face);
}

void WorldIO::genBottomPlane(const std::string& outputDir,
                             std::vector<std::string>& texVec,
                             objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                             glm::ivec3 worldPos, glm::ivec3 objPos,
                             glm::vec3 size, glm::vec3 offset,
                             const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Bottom", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getBottomImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Bottom");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getBottomImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global),
                         ObjIndex(2, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global),
                         ObjIndex(2, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(4, IndexMode::Global),
                         ObjIndex(2, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(3, IndexMode::Global),
                         ObjIndex(2, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(-size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(-size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);
        face.emplace_back(polyD);

        aa.face(face);
}

void WorldIO::genLeftPlane(const std::string& outputDir,
                           std::vector<std::string>& texVec,
                           objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                           glm::ivec3 worldPos, glm::ivec3 objPos,
                           glm::vec3 size, glm::vec3 offset,
                           const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Left", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getLeftImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Left");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getLeftImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(5, IndexMode::Global),
                         ObjIndex(3, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(6, IndexMode::Global),
                         ObjIndex(3, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(7, IndexMode::Global),
                         ObjIndex(3, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(8, IndexMode::Global),
                         ObjIndex(3, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(-size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(-size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(-size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(-size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyD);
        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);

        aa.face(face);
}

void WorldIO::genRightPlane(const std::string& outputDir,
                            std::vector<std::string>& texVec,
                            objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                            glm::ivec3 worldPos, glm::ivec3 objPos,
                            glm::vec3 size, glm::vec3 offset,
                            const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Right", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getRightImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Right");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getRightImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(5, IndexMode::Global),
                         ObjIndex(4, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(6, IndexMode::Global),
                         ObjIndex(4, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(7, IndexMode::Global),
                         ObjIndex(4, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(8, IndexMode::Global),
                         ObjIndex(4, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyD);
        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);

        aa.face(face);
}

void WorldIO::genFrontPlane(const std::string& outputDir,
                            std::vector<std::string>& texVec,
                            objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                            glm::ivec3 worldPos, glm::ivec3 objPos,
                            glm::vec3 size, glm::vec3 offset,
                            const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Front", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getFrontImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Front");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getFrontImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(5, IndexMode::Global),
                         ObjIndex(6, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(6, IndexMode::Global),
                         ObjIndex(6, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(7, IndexMode::Global),
                         ObjIndex(6, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(8, IndexMode::Global),
                         ObjIndex(6, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(-size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(-size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(size.x, size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(size.x, -size.y, size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);
        face.emplace_back(polyD);

        aa.face(face);
}

void WorldIO::genBackPlane(const std::string& outputDir,
                           std::vector<std::string>& texVec,
                           objb::ObjBuilder& ob, objb::MtlBuilder& mb,
                           glm::ivec3 worldPos, glm::ivec3 objPos,
                           glm::vec3 size, glm::vec3 offset,
                           const std::shared_ptr<World>& world) {
        using namespace objb;
        char buf[256];
        std::memset(buf, '\0', 256);
        std::sprintf(buf, "plane%d%d%d_Back", objPos.x, objPos.y, objPos.z);
        auto block = world->getBlock(worldPos.x, worldPos.y, worldPos.z);
        auto texPath = block->getTextureSet().getBackImage()->getPath();
        auto texName = block->getTextureReference() + std::string("_Back");
        auto texFileName = ofFilePath::getFileName(
            block->getTextureSet().getBackImage()->getPath());
        auto copyFile = ofFilePath::join(outputDir, texFileName);

        ObjFace face;

        ObjPolygon polyA(ObjIndex(1), ObjIndex(5, IndexMode::Global),
                         ObjIndex(5, IndexMode::Global));
        ObjPolygon polyB(ObjIndex(2), ObjIndex(6, IndexMode::Global),
                         ObjIndex(5, IndexMode::Global));
        ObjPolygon polyC(ObjIndex(4), ObjIndex(7, IndexMode::Global),
                         ObjIndex(5, IndexMode::Global));
        ObjPolygon polyD(ObjIndex(3), ObjIndex(8, IndexMode::Global),
                         ObjIndex(5, IndexMode::Global));

        auto& aa =
            ob.newModel(buf)
                .sharedVertex(glm::vec3(size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyA)
                .sharedVertex(glm::vec3(size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyB)
                .sharedVertex(glm::vec3(-size.x, size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyC)
                .sharedVertex(glm::vec3(-size.x, -size.y, -size.z) +
                                  asVec3(objPos.x, objPos.y, objPos.z) + offset,
                              polyD);

        if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
                mb.newElement(texName).map_Kd(texFileName);
                texVec.emplace_back(texName);
                auto cwd = ofFilePath::getCurrentExeDir();
                ofFile::copyFromTo(texPath, ofFilePath::join(cwd, copyFile));
        }
        aa.useMaterial(texName);

        face.emplace_back(polyC);
        face.emplace_back(polyA);
        face.emplace_back(polyB);
        face.emplace_back(polyD);

        aa.face(face);
}
}  // namespace planet