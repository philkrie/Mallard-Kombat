/*
 * Duck class definitions
 * Created by Casey Chesshir
 * February 22, 2015
 */

#include "duck.hpp"
#include "mallard.hpp"
#include "beaver.hpp"


Duck::Duck(){
    duckScalar.x = 0;
    duckScalar.y = 350;
    isDead = false;
    footballVisible = false;
    collision = false;
}


void Duck::shootFootball(){
    // initial position of the football
    // near the duck's mouth
    footballScalar.x = (duckScalar.x+70) * Mallard::xcor;
    footballScalar.y = duckScalar.y+10;
    footballScalar.w = 30;
    footballScalar.h = 30;
    footballVisible = true;
}

void Duck::renderDuck(SDL_Renderer *ren, int count){
    if (footballVisible) {
        footballScalar.x +=10 * Mallard::xcor;
        SDL_RenderCopy(ren, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(ren, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640 * Mallard::xcor || collision){
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