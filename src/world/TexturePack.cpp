#include "TexturePack.hpp"

#include <ofUtils.h>

#include <algorithm>

#include "Block.hpp"
#include "BlockPack.hpp"
#include "Plane.hpp"
namespace planet {

std::vector<std::shared_ptr<TexturePack> > TexturePack::texturePacks;

std::shared_ptr<TexturePack> TexturePack::load(
    const TextureInfoCollection& textureInfoCollection) {
        auto tp = new TexturePack(textureInfoCollection.getBaseDirectory());
        auto ret = std::shared_ptr<TexturePack>(tp);
        // load texturesets
        for (int i = 0; i < textureInfoCollection.getTextureInfoCount(); i++) {
                auto tinfo = textureInfoCollection.getTextureInfo(i);
                TextureSet set(ret, tinfo.reference);
                if (tinfo.mappingRule.all.hasValue()) {
                        set.top = set.bottom = set.front = set.back = set.left =
                            set.right = (tinfo.baseFileName +
                                         tinfo.mappingRule.all.getValue());
                }
                if (tinfo.mappingRule.top.hasValue()) {
                        set.top = (tinfo.baseFileName +
                                   tinfo.mappingRule.top.getValue());
                }
                if (tinfo.mappingRule.bottom.hasValue()) {
                        set.bottom = (tinfo.baseFileName +
                                      tinfo.mappingRule.bottom.getValue());
                }
                if (tinfo.mappingRule.front.hasValue()) {
                        set.front = (tinfo.baseFileName +
                                     tinfo.mappingRule.front.getValue());
                }
                if (tinfo.mappingRule.back.hasValue()) {
                        set.back = (tinfo.baseFileName +
                                    tinfo.mappingRule.back.getValue());
                }
                if (tinfo.mappingRule.left.hasValue()) {
                        set.left = (tinfo.baseFileName +
                                    tinfo.mappingRule.left.getValue());
                }
                if (tinfo.mappingRule.right.hasValue()) {
                        set.right = (tinfo.baseFileName +
                                     tinfo.mappingRule.right.getValue());
                }
                ret->addTextureSet(set);
        }
        // load textures
        for (TextureSet set : ret->textureSets) {
                auto topFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.top);
                auto topTex = std::make_shared<Texture>();
                topTex->load(topFile + ".png");
                auto bottomFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.bottom);
                auto bottomTex = std::make_shared<Texture>();
                bottomTex->load(bottomFile + ".png");
                auto leftFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.left);
                auto leftTex = std::make_shared<Texture>();
                leftTex->load(leftFile + ".png");
                auto rightFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.right);
                auto rightTex = std::make_shared<Texture>();
                rightTex->load(rightFile + ".png");
                auto frontFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.front);
                auto frontTex = std::make_shared<Texture>();
                frontTex->load(frontFile + ".png");
                auto backFile = ofFilePath::join(
                    textureInfoCollection.getBasePath(), set.back);
                auto backTex = std::make_shared<Texture>();
                backTex->load(backFile + ".png");

                tp->images.insert_or_assign(set.top, topTex);
                tp->images.insert_or_assign(set.bottom, bottomTex);
                tp->images.insert_or_assign(set.left, leftTex);
                tp->images.insert_or_assign(set.right, rightTex);
                tp->images.insert_or_assign(set.front, frontTex);
                tp->images.insert_or_assign(set.back, backTex);
        }
        return registerTexturePack(ret);
}

std::shared_ptr<TexturePack> TexturePack::make(const std::string& baseDirectory,
                                               const std::string& name) {
        auto tp = new TexturePack(baseDirectory);
        tp->name = name;
        return registerTexturePack(std::shared_ptr<TexturePack>(tp));
}

std::shared_ptr<TexturePack> TexturePack::getCurrent() {
        for (auto tp : texturePacks) {
                if (tp->isSelected()) {
                        return tp;
                }
        }
        return nullptr;
}

void TexturePack::addTextureSet(const TextureSet& set) {
        this->textureSets.emplace_back(set);
}

int TexturePack::getTextureSetIndexForName(const std::string& name) const {
        int i = 0;
        for (const TextureSet& set : textureSets) {
                if (set.getName() == name) {
                        return i;
                }
                ++i;
        }
        return -1;
}

const TextureSet& TexturePack::getTextureSet(int index) const {
        return textureSets.at(index);
}

void TexturePack::removeTextureSet(int index) {
        textureSets.erase(textureSets.begin() + index);
}

int TexturePack::getTextureSetCount() const {
        return static_cast<int>(textureSets.size());
}

std::shared_ptr<Texture> TexturePack::getImage(const std::string& name) const {
        return images.at(name);
}

bool TexturePack::isSelected() const { return selected; }

void TexturePack::select() {
        if (selected) {
                return;
        }
        for (auto tp : TexturePack::texturePacks) {
                tp->selected = false;
        }
        this->selected = true;
        resolve();
}

void TexturePack::resolve() {
        if (!isSelected()) {
                throw std::logic_error("this pack is not selected");
        }
        auto bp = BlockPack::getCurrent();
        for (int i = 0; i < bp->getBlockCount(); i++) {
                auto block = bp->getBlock(i);
                int textureSetIndex =
                    getTextureSetIndexForName(block->getTextureReference());
                if (textureSetIndex == -1) {
                        continue;
                }
                block->setTextureSetIndex(textureSetIndex);
        }
}

std::string TexturePack::getBaseDirectory() const { return baseDirectory; }

std::string TexturePack::getBasePath() const {
        return ofToDataPath(baseDirectory);
}

// protected
TexturePack::TexturePack(const std::string& baseDirectory)
    : selected(false),
      baseDirectory(baseDirectory),
      name(),
      textureSets(),
      images() {}

std::shared_ptr<TexturePack> TexturePack::registerTexturePack(
    const std::shared_ptr<TexturePack> pack) {
        bool empty = texturePacks.empty();
        texturePacks.emplace_back(pack);
        if (empty) {
                pack->select();
        }
        return pack;
}

// private
}  // namespace planet
