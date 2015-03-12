#ifndef MALLARD_HPP
#define MALLARD_HPP

#include <SDL2/SDL.h>   
#include <SDL2/SDL_image.h>                    // SDL library
#include <SDL2/SDL_ttf.h>                   // SDL font library
#include <SDL2/SDL_mixer.h>                 // SDL sound library
#include "enemy.hpp"
#include "duck.hpp"
#include "beaver.hpp"
#include "trojan.hpp"
#include "husky.hpp"
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
    
    //fstream used to read the highscore file
    std::fstream file;

    // path to the image files
    std::string path; 

    // Window and renderer
    SDL_Window* window;
    SDL_Renderer *renderer;
    
    // font attributes
    SDL_Color font_color;
    std::string font_name;
    SDL_Texture *text;
    SDL_Rect textRect;
    SDL_Rect pauseRect;
    
    // highscore management
    int highscores[5];
    SDL_Texture *highTextures[5];
    SDL_Texture *blank;
    SDL_Rect highRect;
    bool scoresVisible;
    
    // max score is 18,446,744,073,709,551,615
    // 18 pentillion. Try to beat it!
    long long unsigned int score;
    
    // TST stands for TitleSurfaceTextures
    SDL_Texture *TST[5];
    SDL_Texture *controls;

    //Enemy textures and storage
    Enemy *enemyArray[5];
    SDL_Surface *first_stage_surface;
    SDL_Texture *first_stage_texture;
    SDL_Texture *beaverSkin;
    SDL_Texture *huskySkin;
    SDL_Texture *trojanSkin;
    SDL_Texture *footballSkin;
    SDL_Texture *pauseTexture;
    SDL_Texture *losingText;
    int spawnCount;
    int count;

    // bools for the title screen
    bool on_start;
    bool on_controls;
    bool on_highscores;
    bool on_quit;
    void getBools(int, int);
    
    // bools for which screen is visible
    bool title_visible;
    bool first_stage_visible;
    bool paused;
    
    // render functions
    void render_title_screen();
    void render_first_stage();
    void render_blank_screen();

    //Write score to highscore file
    void recordScore(int);
    
    //Resets game
    void reset();

    //collision function
    bool didCollide(SDL_Rect, SDL_Rect);
    
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
