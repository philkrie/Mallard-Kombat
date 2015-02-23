/*
 *  Pong class declaration
 */

#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2_ttf/SDL_ttf.h>                   // SDL font library
#include <SDL2_mixer/SDL_mixer.h>                 // SDL sound library
#include <SDL2_image/SDL_image.h>

#include <iostream>

class Mallard {
public:
    // Screen resolution
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    SDL_Window* window;                     // holds window properties


    Mallard(int argc, char *argv[]);
    void execute();
    void input();
    void update();
    void render();
    void clean_up();
};

#endif
