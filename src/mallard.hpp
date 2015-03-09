#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>                       // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include <SDL2/SDL_image.h>
#include "duck.hpp"
#include "beaver.hpp"
#include "util.hpp"

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>


#define PI 3.141592653

class Duck;
class Beaver;

class Mallard {
public:
    // Screen resolution
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    
    static double xcor;
    // resolution correction factors
    static double ycor;
    
    std::fstream file;


    // Window and renderer
    SDL_Window* window;
    SDL_Renderer *renderer;
    std::string path; // path to the image files
    
    // font things
    SDL_Color font_color;
    std::string font_name;
    SDL_Texture *swag;
    SDL_Rect swagRect;
    
    // highscores things
    SDL_Texture *highTextures[3];
    SDL_Texture *blank;
    SDL_Rect highRect;
    bool scoresVisible;
    
    long long unsigned int score;
    // max score is 18,446,744,073,709,551,615
    // 18 pentillion
    
    SDL_Texture *TST[5];  // TST stands for TitleSurfaceTextures
    
    Beaver *beaverArray[5];
    
    SDL_Surface *first_stage_surface;
    SDL_Texture *first_stage_texture;
    // bools for the title screen
    
    bool on_start;
    bool on_options;
    bool on_highscores;
    bool on_quit;
    void getBools(int, int);
    int spawnCount;
    int count;
    
    
    // bools for which screen is visible
    bool title_visible;
    bool first_stage_visible;
    bool paused;
    
    // render functions
    void render_title_screen();
    void render_first_stage();
    void render_blank_screen();

    void recordScore(int);
    
    void reset();

    int highscores[3];
    //collision function
    bool didCollide(SDL_Rect, SDL_Rect);
    int gameBreaker;
    // ^ this is necessary for the game to run
    
    // class objects
    Duck *duck;
    Beaver *beaver;
    
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
