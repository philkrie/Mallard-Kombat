/*
 * Duck class definitions
 * Created by Casey Chesshir
 * March 7, 2015
 */
#ifndef BEAVER_HPP_
#define BEAVER_HPP_

#include "mallard.hpp"
#include "enemy.hpp"

#include <SDL2/SDL.h>   
#include <SDL2/SDL_image.h>                    // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class Beaver: public Enemy {
public:
    Beaver(int, int);
    ~Beaver();
    
    int spawnPoint;
    void respawn();
    void render(SDL_Renderer *);
    void move();
};




#endif