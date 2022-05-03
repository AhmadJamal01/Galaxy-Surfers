#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //TODO: (Req 10) Finish this function to create an empty texture with the given size and format
    
    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();                     //* This will create the texture (generare ID)
    //Bind the texture such that we upload the image data to its storage
    //TODO: (Req 4) Finish this function to fill the texture with the data found in "pixels" and generate the mipmaps if requested
    texture->bind();                                                    //* Now the texture is bound.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixels);
    //@fun glTexImage2D: specify a two-dimensional texture image (Attach it to bound texture object on GPU)
    //@param GL_TEXTURE_2D: The target texture
    //@param 0: Mipmap base level of 0
    //@param GL_RGBA8: internal format that will be used to store the image (check like 28, 29)
    //@param size.x: width of the texture
    //@param size.y: height of the texture
    //@param 0: must be 0 for compatibility 
    //@param GL_RGBA: format of source image
    //@param GL_UNSIGNED_BYTE: Datatype of pixel data
    //@param pixels: Pointer to the image(texture) data in memory.


    if(generate_mipmap)     glGenerateMipmap(GL_TEXTURE_2D);
    //@fun glGenerateMipMap: generate mipmaps for a specified texture objec
    //@param GL_TEXTURE_2D: specifies the target to which the texture object is bound
    //Mipmaps will be dynamically use for minification and magnification.

    stbi_image_free(pixels); //Free image data after uploading to GPU

    return texture;
}