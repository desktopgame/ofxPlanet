#pragma once
#ifndef WORLD_TEXTURE_HPP
#define WORLD_TEXTURE_HPP
#include <ofTypes.h>

#include <string>

namespace ofxPlanet {
/**
 * Texture is wrapper for OpenGL texture function.
 */
class Texture {
       public:
        explicit Texture();
        ~Texture();
		/** 
		 * load texture file for specific path.
		 * @param path
		 */
        void load(const std::string& path);
		/**
		 * bind this texture.
		 */
        void bind();
		/**
		 * unbind texture.
		 */
        void unbind();

		/**
		 * returns width of texture.
		 * @return
		 */
        int getWidth() const;
		/**
		 * returns height of texture.
		 * @return
		 */
        int getHeight() const;
		/**
		 * returns RGBA data for texture.
		 * @return
		 */
        unsigned char* getData() const;

		/**
		 * returns texture object in OpenGL.
		 * @return
		 */
        GLuint getName() const;

		/**
		 * returns path for texture.
		 * @return
		 */
        std::string getPath() const;

       private:
        Texture(const Texture& obj) = delete;
        Texture& operator=(const Texture&) = delete;
        std::string path;
        int width, height, ch;
        unsigned char* data;
        GLuint name;
};
}  // namespace ofxPlanet
#endif