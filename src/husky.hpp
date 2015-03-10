//
//  husky.hpp
//  
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#ifndef HUSKY_HPP_
#define HUSKY_HPP_

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>                      // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class Husky{
public:
    Husky(int, int);
    ~Husky();
    
    SDL_Texture *huskyTexture;
    SDL_Rect huskyScalar;
    SDL_Texture *beaverFootball;
    
    int spawnPoint;
    bool hasFootball;
    void respawn();
    void renderHusky(SDL_Renderer *);
};
#endif /* defined(____husky__) */
