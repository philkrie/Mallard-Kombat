#ifndef BEAVER_HPP_
#define BEAVER_HPP_

#include "mallard.hpp"

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class Beaver{
public:
    
    SDL_Surface *beaverSurface;
    SDL_Texture *beaverTexture;
    SDL_Rect beaverScalar;
    
    bool beaverVisible;
    int spawnPoint;
    int beaverCount;
    void respawn();
    Beaver();
};




#endif