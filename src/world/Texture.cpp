#include "Texture.hpp"

#include <ofxSOIL.h>

namespace planet {

Texture::Texture() : width(-1), height(-1), ch(-1), data(nullptr), name(0) {
        glGenTextures(1, &name);
}

Texture::~Texture() {
        glDeleteTextures(1, &name);
        std::free(data);
}

void Texture::load(const std::string& path) {
        ofxSOIL::Image img = ofxSOIL::loadImage(path, ofxSOIL::RGBA);
        this->path = path;
        this->data = img.data;
        this->width = img.width;
        this->height = img.height;
        this->ch = img.channel;
        glBindTexture(GL_TEXTURE_2D, name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, name); }

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

int Texture::getWidth() const { return width; }

int Texture::getHeight() const { return height; }

unsigned char* Texture::getData() const { return data; }

GLuint Texture::getName() const { return name; }
std::string Texture::getPath() const { return path; }
}  // namespace planet
