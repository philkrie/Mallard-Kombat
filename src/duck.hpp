/*
 * Duck class declaration
 * Created by Casey Chesshir
 * February 22, 2015
 */

#ifndef DUCK_HPP
#define DUCK_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
class Beaver;

class Duck{
public:
    Duck();
    ~Duck();
    SDL_Texture *DST[4];  // DST stands for DuckSurfaceTextures
    
    SDL_Texture *footballTexture;
    SDL_Rect duckScalar;
    SDL_Rect footballScalar;

    void jump();
    void renderDuck(SDL_Renderer *, int);
    void shootFootball();
    void renderFootball();
    
    bool footballVisible;
    bool collision;
    Mix_Chunk *quack;
    
    bool isDead;

    
};


#endif