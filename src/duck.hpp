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
public:
    Duck();
    ~Duck();
    SDL_Surface *DSS[4];  // DSS stands for DuckScreenSurfaces
    SDL_Surface *CDSS[4]; // CDSS stands for ConvertedDuckScreenSurfaces
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