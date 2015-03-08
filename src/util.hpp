#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* createTexture(const char*, SDL_Renderer*);
SDL_Texture* renderText(const std::string, const std::string, SDL_Color, int, SDL_Renderer *);

#endif
