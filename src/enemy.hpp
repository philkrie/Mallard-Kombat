//
//  enemy.hpp
//  pure virtual class
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class Enemy{
public:
    bool hasFootball;
    SDL_Rect enemyScalar;
    SDL_Rect footballScalar;
    SDL_Texture *enemyTexture;
    SDL_Texture *footballTexture;
    virtual void respawn() = 0;
    virtual void render(SDL_Renderer *) = 0;
    virtual void move() = 0;
};


#endif