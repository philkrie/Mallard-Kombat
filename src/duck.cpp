/*
 * Duck class definitions
 * Created by Casey Chesshir
 * February 22, 2015
 */

#include "duck.hpp"
#include "mallard.hpp"
#include "beaver.hpp"

//The duck is controlled by the player and fires the football
Duck::Duck(){
    duckScalar.x = 0;
    duckScalar.y = 350;
    isDead = false;
    footballVisible = false;
    collision = false;
}


void Duck::shootFootball(){
    footballScalar.x = int (duckScalar.x+70);
    footballScalar.y = duckScalar.y+20;
    footballScalar.w = 20;
    footballScalar.h = 14;
    footballVisible = true;
}

void Duck::renderDuck(SDL_Renderer *ren, int count){
    if (footballVisible) {
        footballScalar.x +=10;
        SDL_RenderCopy(ren, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(ren, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640|| collision){
            footballVisible = false;
            collision = false;
        }
        
    }
    else if (isDead) {
        SDL_RenderCopy(ren, DST[3], NULL, &duckScalar);
    }
    
    else if (count <= 15){
        SDL_RenderCopy(ren, DST[2], NULL, &duckScalar);
    }
    else if (count < 30){
        SDL_RenderCopy(ren, DST[0], NULL, &duckScalar);
    }
}