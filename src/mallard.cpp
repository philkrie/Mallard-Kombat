#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "mallard.hpp"
#include "duck.hpp"

/* Screen resolution */
const int Mallard::SCREEN_WIDTH = 640;
const int Mallard::SCREEN_HEIGHT = 480;

enum LButtonSprite
{
    START = 0,
    OPTIONS = 1,
    CREDITS = 2,
    QUIT = 3,
};

Mallard::Mallard(int argc, char* argv[]) {
    exit = false;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    //SDL_ShowCursor(0);         // Initialize SDL2
    



    // Create an application window with the following settings:
    this->window = SDL_CreateWindow( "MALLARD KOMBAT",                  // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              640,                               // width, in pixels
                              480,                               // height, in pixels
                              SDL_WINDOW_SHOWN);                 // flags - see below

    // Check that the window was successfully made
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED |
                                 SDL_RENDERER_PRESENTVSYNC );
    path = "/resources/images/";
    //TS stands for TitleScreens
    std::string TS[5] = {
    "title_screen",
    "title_screen_start",
    "title_screen_options",
    "title_screen_credits",
    "title_screen_quit",
    };
    // Creating the title screens
    
    //TSS stands for TitleScreenSurfaces
    
    for (int i=0; i < 5; i++) {
        std::string filepath = path + TS[i] + ".bmp";
        char *temp = (char*)filepath.c_str();
        TSS[i] = SDL_LoadBMP(temp);
        CTSS[i] = SDL_ConvertSurfaceFormat(TSS[i], SDL_PIXELFORMAT_RGBA8888, 0);
        TST[i] = SDL_CreateTextureFromSurface(renderer, CTSS[i]);
        SDL_FreeSurface(CTSS[i]);
    }
    
    //SDL_FreeSurface(title_screen);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, TST[0], NULL, NULL);
    SDL_RenderPresent(renderer);
    
    Duck *duck;
    duck = new Duck(40, 50);
}

void Mallard::input(){
    int x, y;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&x, &y);
            bool on_start = (450 < x && x < 550) && (275 < y && y < 300);
            bool on_options = (435 < x && x < 565) && (320 < y && y < 345);
            bool on_credits = (435 < x && x < 565) && (365 < y && y < 385);
            bool on_quit = (465 < x && x < 535) && (410 < y && y < 430);
            if (on_start) {
                SDL_RenderCopy(renderer, TST[1], NULL, NULL);
                SDL_RenderPresent(renderer);
            }
            else if (on_options) {
                SDL_RenderCopy(renderer, TST[2], NULL, NULL);
                SDL_RenderPresent(renderer);
            }
            else if (on_credits) {
                SDL_RenderCopy(renderer, TST[3], NULL, NULL);
                SDL_RenderPresent(renderer);
            }
            else if (on_quit) {
                SDL_RenderCopy(renderer, TST[4], NULL, NULL);
                SDL_RenderPresent(renderer);
            }else{
                SDL_RenderCopy(renderer, TST[0], NULL, NULL);
                SDL_RenderPresent(renderer);
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            exit = true;
        }
    }
    
}

void Mallard::update(){
    
}

void Mallard::render(){
    
}

void Mallard::execute() {
    
    while(!exit) {
        input();
        update();
        render();
        SDL_Delay(10);
    }
    std::cout << "preparing to Quit" << std::endl;
    clean_up();
    std::cout << "successfully Quit" << std::endl;
}

void Mallard::clean_up(){
    for (int i=0; i < 5; i++) {
        SDL_DestroyTexture(TST[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}