#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "mallard.hpp"

/* Screen resolution */
const int Mallard::SCREEN_WIDTH = 640;
const int Mallard::SCREEN_HEIGHT = 480;

Mallard::Mallard(int argc, char* argv[]) {
    
    SDL_Window *window;                    // Declare a pointer
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_ShowCursor(0);         // Initialize SDL2
    




    // Create an application window with the following settings:
    window = SDL_CreateWindow( "MALLARD KOMBAT",                  // window title
                              SDL_WINDOWPOS_UNDEFINED,           // initial x position
                              SDL_WINDOWPOS_UNDEFINED,           // initial y position
                              640,                               // width, in pixels
                              480,                               // height, in pixels
                              SDL_WINDOW_SHOWN);                 // flags - see below

    
    // Check that the window was successfully made
   
    
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
    SDL_DestroyWindow(window);
    SDL_Quit();

}