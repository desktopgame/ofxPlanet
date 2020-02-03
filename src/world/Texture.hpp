#pragma once
#ifndef WORLD_TEXTURE_HPP
#define WORLD_TEXTURE_HPP
#include <ofTypes.h>

#include <string>

namespace planet {
/**
 * Texture は、テクスチャをOpenGLと紐付けるクラスです。
 */
class Texture {
       public:
        explicit Texture();
        ~Texture();
        void load(const std::string& path);

        void bind();
        void unbind();

        int getWidth() const;
        int getHeight() const;

        unsigned char* getData() const;
        GLuint getName() const;
        std::string getPath() const;

       private:
        Texture(const Texture& obj) = delete;
        Texture& operator=(const Texture&) = delete;
        std::string path;
        int width, height, ch;
        unsigned char* data;
        GLuint name;
};
}  // namespace planet
#endif