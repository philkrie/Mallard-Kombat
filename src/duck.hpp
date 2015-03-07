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

class Beaver;

class Duck{
private:
    int x;
    int y;
    int xspeed;
    int yspeed;
public:
    Duck();
    int get_x();
    int get_y();
    void set_x(int new_x);
    void set_y(int new_y);
    void set_xspeed(int new_x);
    void set_yspeed(int new_y);
    
    SDL_Texture *DST[4];  // DST stands for DuckSurfaceTextures
    SDL_Texture* footballTexture;
    
    SDL_Rect duckScalar;
    SDL_Rect footballScalar;


    void jump();
    void renderDuck(SDL_Renderer*, int);
    void shootFootball();
    bool footballVisible;
    bool collision;

    
    Mix_Chunk *quack;
    
    bool isDuckDead;

};


#endif