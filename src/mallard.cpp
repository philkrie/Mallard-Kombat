#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "mallard.hpp"
#include "duck.hpp"

/* Screen resolution */
const int Mallard::SCREEN_WIDTH = 640;
const int Mallard::SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    START = 0,
    OPTIONS = 1,
    CREDITS = 2,
    QUIT = 3,
};

Mallard::Mallard(int argc, char* argv[]) {
    
    SDL_Window *window;                    // Declare a pointer
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(0);         // Initialize SDL2
    



    // Create an application window with the following settings:
    window = SDL_CreateWindow( "MALLARD KOMBAT",                  // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              640,                               // width, in pixels
                              480,                               // height, in pixels
                              SDL_WINDOW_SHOWN);                 // flags - see below

    SDL_Renderer *renderer;
    // Check that the window was successfully made
   renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED |
                                 SDL_RENDERER_PRESENTVSYNC );
    
    
    // Creating the title screen
    SDL_Surface *title_screen = SDL_LoadBMP("/resources/images/title_screen.bmp");
    if (title_screen == NULL) {
        // this happens if title_screen's LoadBMP failed
        std::cout << SDL_GetError() << std::endl;
    }
    SDL_Surface *title_screen_surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);
    
    SDL_Texture * title_screen_texture = SDL_CreateTextureFromSurface(renderer, title_screen);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    
    SDL_FreeSurface(title_screen);
    Duck *duck;
    duck = new Duck(40, 50);
    // The window is open: enter program loop (see SDL_PollEvent)
    
     // Pause execution for 3000 milliseconds, for example
    
    // Close and destroy the window
    
    // Clean up
}

void Mallard::execute() {
    
    /*while(!exit) {
        input();
        update();
        render();
        SDL_Delay(10);
    }*/

    SDL_Delay(3000);
    std::cout << "preparing to Quit" << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "successfully Quit" << std::endl;
}