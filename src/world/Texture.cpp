#include "Texture.hpp"

#include <ofUtils.h>
#include <ofxSOIL.h>

namespace ofxPlanet {

Texture::Texture() : name(), width(-1), height(-1), data(nullptr) {
        glGenTextures(1, &name);
}

Texture::~Texture() {
        glDeleteTextures(1, &name);
        std::free(data);
}

void Texture::load(const std::string& path) {
        std::string cp = path;
        if (cp.substr(0, 5) == "data\\" || cp.substr(0, 5) == "data/") {
                cp = cp.substr(5);
        }
        ofxSOIL::Image img = ofxSOIL::loadImage(path, ofxSOIL::RGBA);
        this->path = path;
        this->data = img.data;
        this->width = img.width;
        this->height = img.height;
        glBindTexture(GL_TEXTURE_2D, name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, getName()); }

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

int Texture::getWidth() const { return width; }

int Texture::getHeight() const { return height; }

unsigned char* Texture::getData() const { return nullptr; }

GLuint Texture::getName() const { return name; }
std::string Texture::getPath() const { return path; }
}  // namespace ofxPlanet
