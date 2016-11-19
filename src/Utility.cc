#include "utility.h"
#include <iostream>

bool loadTexture(char *filename, GLuint &texture)
{
    SDL_Surface *surface;
    GLenum texture_format;
    GLint  nOfColors;

    if ((surface = SDL_LoadBMP(filename))) {
        if ((surface->w & (surface->w - 1)) != 0) {
            printf("warning: %s's width is not a power of 2\n", filename);
        }
        if ((surface->h & (surface->h - 1)) != 0) {
            printf("warning: %s's height is not a power of 2\n", filename);
        }

        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4){
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        } else if (nOfColors == 3){
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                          texture_format, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);
    } else {
        printf("Failed to load Texture: %s\n", filename);
        return false;
    }
    return true;
} // end loadTexture method
