/*
 *  Pong class declaration
 */

#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

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
    
    SDL_Surface *first_stage_surface;
    SDL_Texture *first_stage_texture;
    SDL_Surface *duckSurface;
    SDL_Texture *duckTexture;
    
    SDL_Rect scaler;
    // bools for the title screen
    
    bool on_start;
    bool on_options;
    bool on_credits;
    bool on_quit;
    bool jumping;
    void getBools(int, int);

    int yspeed;
    int xspeed;
    
    // bools for which screen is visible
    bool title_visible;
    bool first_stage_visible;
    
    // render functions
    void render_title_screen();
    void render_first_stage();
    
    // Sounds
    
    Mix_Chunk *quack;
    Mallard(int argc, char *argv[]);
    void execute();
    void input();
    void update();
    void render();
    void clean_up();
    void jump();
    bool exit;
};

#endif
