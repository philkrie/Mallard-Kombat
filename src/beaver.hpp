/*
 * Duck class definitions
 * Created by Casey Chesshir
 * March 7, 2015
 */
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
    Beaver(int, int);
    ~Beaver();
    
    SDL_Texture *beaverTexture;
    SDL_Rect beaverScalar;
    SDL_Texture *beaverFootball;
    SDL_Rect beaverFootballScalar;
    
    int spawnPoint;
    bool beaverVisible;
    bool hasFootball;
    void respawn();
    void renderBeaver(SDL_Renderer *);
};




#endif