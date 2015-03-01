/*
 * Duck class declaration
 * Created by Casey Chesshir
 * February 22, 2015
 */

#ifndef DUCK_HPP
#define DUCK_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

class Duck{
private:
    int x;
    int y;
    int xspeed;
    int yspeed;
public:
    Duck(int x, int y);
    int get_x();
    int get_y();
    void set_x(int new_x);
    void set_y(int new_y);
    void set_xspeed(int new_x);
    void set_yspeed(int new_y);
    
    SDL_Surface *duckSurface;
    SDL_Texture *duckTexture;
    SDL_Rect duckScalar;
    void jump();
    void renderDuck();
    void shootFootball();
    void renderFootball();
    
    Mix_Chunk *quack;
    
};


#endif