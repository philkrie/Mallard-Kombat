#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <string>

#include "beaver.hpp"
#include "duck.hpp"
#include "util.hpp"

#define PI 3.141592653

class Beaver;
class Duck;

class Mallard {
public:
    // Screen resolution
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    
    Mallard(int argc, char *argv[]);
    void execute();
    void input();
    void update();
    void render();
    void clean_up();
    

    // Window and renderer
    SDL_Window* window;
    SDL_Renderer *renderer;

    // path to the image files
    std::string path; 

    // gameobjects
    Duck *duck;
    Beaver *benny;
        
    // TTF Font Library
    SDL_Color font_color;
    std::string font_name;
    SDL_Texture *swag;
    SDL_Rect swagRect;
   
    
    SDL_Surface *first_stage_surface;
    SDL_Texture *first_stage_texture;
    
    SDL_Surface *footballSurface;
    SDL_Texture *footballTexture;

    SDL_Texture *TST[5];
    
   
    
    // bools for the title screen
    
    bool on_start;
    bool on_options;
    bool on_credits;
    bool on_quit;
    bool jumping;
    void getBools(int, int);
    int count;
    long long unsigned int score;
    bool exit;
    
    //bools for the first stage
    bool beaverVisible;
    double yspeed;
    double xspeed;
    
    // bools for which screen is visible
    bool title_visible;
    bool first_stage_visible;
    bool paused;
    // render functions
    void render_title_screen();
    void render_first_stage();
    
    // duck functions
    void jump();
    void shootFootball();
    void renderFootball();
    
    // beaver functions
    void beaverRespawn();
    int beaverStartPoint;
    
    //collision function
    bool didCollide(SDL_Rect, SDL_Rect);
    int gameBreaker;
    // ^ this is necessary for the game to run
    
    // class objects
   
    // Sounds
    Mix_Chunk *quack;
    
};

#endif
