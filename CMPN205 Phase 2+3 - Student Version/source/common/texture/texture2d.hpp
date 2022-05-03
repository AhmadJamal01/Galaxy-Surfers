#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: (Req 4) Complete this function
            glGenTextures(1, &name);                 // generate an ID for 1 texture
            //@fun glDeleteTextures: generate texture names
            //@param 1: number of textures to be create
            //@param &name: Array of where texture names should be stored
            //! Active texture unit will be the zeroth unless we specify another one

        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //TODO: (Req 4) Complete this function
            glDeleteTextures(1, &name);
            //@fun glDeleteTextures: delete named textures
            //@param 1: number of textures to be deleted
            //@param &name: Array of texture names for which the corresponding textures should be deleted.

        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: (Req 4) Complete this function
            glBindTexture(GL_TEXTURE_2D, name);
            //@fun glBindTexture: bind a named texture to a texturing target
            //@param 1: Specifies the target to which the texture should be bound
            //@param name: The name of the texture to be bound
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //TODO: (Req 4) Complete this function
            glBindTexture(GL_TEXTURE_2D, 0);
            //* It will unbind the currently bound texture because we passed 0.

        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}