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
    
    // Window and renderer
    SDL_Window* window;
    SDL_Renderer *renderer;
    std::string path; // path to the image files
    
    SDL_Surface *TSS[5];  // TSS stands for TitleScreenSurfaces
    SDL_Surface *CTSS[5]; // CTSS stands for ConvertedTitleScreenSurfaces
    SDL_Texture *TST[5];  // TST stands for TextureSurface
    
    // Sounds
    
    Mix_Chunk *quack;
    Mallard(int argc, char *argv[]);
    void execute();
    void input();
    void update();
    void render();
    void clean_up();
    bool exit;
};

#endif
