#include "util.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

SDL_Texture* createTexture(const char* path, SDL_Renderer *ren){
    SDL_Surface *temp = IMG_Load(path);
    temp = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_Texture *result = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return result;
}

SDL_Texture* renderText(const std::string message, const std::string fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    SDL_Surface *temp = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *result = SDL_CreateTextureFromSurface(ren, temp);

    SDL_FreeSurface(temp);
    TTF_CloseFont(font);
    return result;
}