#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <random>

class Mallard {
public:
    // Screen resolution
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    
    // Window and renderer
    SDL_Window* window;
    SDL_Renderer *renderer;
    std::string path; // path to the image files
    
    // font things
    TTF_Font *font;
    SDL_Color font_color;
    std::string font_name;
    SDL_Texture *swag;
    SDL_Surface *surfswag;
    SDL_Texture *renderText(const std::string&, const std::string&, SDL_Color, int, SDL_Renderer*);
    SDL_Rect swagRect;
    
    SDL_Surface *TSS[5];  // TSS stands for TitleScreenSurfaces
    SDL_Surface *CTSS[5]; // CTSS stands for ConvertedTitleScreenSurfaces
    SDL_Texture *TST[5];  // TST stands for TextureSurface

    SDL_Surface *DSS[3];  // TSS stands for TitleScreenSurfaces
    SDL_Surface *CDSS[3]; // CTSS stands for ConvertedTitleScreenSurfaces
    SDL_Texture *DST[3];  // TST stands for TextureSurface
    
    SDL_Surface *first_stage_surface;
    SDL_Texture *first_stage_texture;
    SDL_Surface *duckSurface;
    SDL_Texture *duckTexture;
    
    SDL_Surface *footballSurface;
    SDL_Texture *footballTexture;
    
    SDL_Surface *beaverSurface;
    SDL_Texture *beaverTexture;
    
    SDL_Rect footballScalar;
    SDL_Rect beaverScalar;
    SDL_Rect duckScalar;
    // bools for the title screen
    
    bool on_start;
    bool on_options;
    bool on_credits;
    bool on_quit;
    bool jumping;
    void getBools(int, int);
    int count;
    
    //bools for the first stage
    bool footballVisible;
    bool beaverVisible;
    
    double yspeed;
    double xspeed;
    
    // bools for which screen is visible
    bool title_visible;
    bool first_stage_visible;
    // render functions
    void render_title_screen();
    void render_first_stage();
    
    // duck functions
    void jump();
    void shootFootball();
    void renderFootball();
    
    // beaver functions
    int beaverRespawn();
    
    //collision function
    bool didCollide(SDL_Rect, SDL_Rect);
    
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
