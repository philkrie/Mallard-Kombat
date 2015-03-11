//
//  husky.hpp
//  
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#ifndef HUSKY_HPP_
#define HUSKY_HPP_
                    // SDL library
#include "enemy.hpp"
#include "mallard.hpp"

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>


class Husky: public Enemy{
public:
    Husky(int, int);
    ~Husky();
   
    void respawn();
    void render(SDL_Renderer *);
    void move();
};
#endif /* defined(____husky__) */
