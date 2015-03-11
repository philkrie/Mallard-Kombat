//
//  husky.hpp
//  
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#ifndef TROJAN_HPP_
#define TROJAN_HPP_

#include "enemy.hpp"
#include "mallard.hpp"

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

class Trojan: public Enemy{
public:
    Trojan(int, int);
    ~Trojan();
   
    void respawn();
    void render(SDL_Renderer *);
    void move();
};
#endif /* defined(____husky__) */
