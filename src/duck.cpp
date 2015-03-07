/*
 * Duck class definitions
 * Created by Casey Chesshir
 * February 22, 2015
 */

#include "duck.hpp"
#include "mallard.hpp"

Duck::Duck(){
    duckScalar.x = 0;
    duckScalar.y = 350;
    isDuckDead = false;
    yspeed = 0;
    collision = false;
}

int Duck::get_x(){
    return x;
}

int Duck::get_y(){
    return y;
}

void Duck::set_y(int new_y){
    y = new_y;
}

void Duck::jump(){
    int speed = 20;
    duckScalar.y -= speed;
    speed--;
    for (int i = 0; i < 20; i++) {
        duckScalar.y += speed;
        speed++;
    }
}

void Duck::shootFootball(){
    // initial position of the football
    // near the duck's mouth
    footballVisible = true;
    footballScalar.x = duckScalar.x+70;
    footballScalar.y = duckScalar.y+10;
    footballScalar.w = 30;
    footballScalar.h = 30;
}

void Duck::renderDuck(SDL_Renderer *ren, int count){
    if (footballVisible) {
        footballScalar.x +=10;
        SDL_RenderCopy(ren, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(ren, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640 || collision){
            footballVisible = false;
            collision = false;
        }
    }
    else if (isDuckDead) {
        SDL_RenderCopy(ren, DST[3], NULL, &duckScalar);
    }
    
    else if (count <= 15){
        SDL_RenderCopy(ren, DST[2], NULL, &duckScalar);
    }
    else if (count < 30){
        SDL_RenderCopy(ren, DST[0], NULL, &duckScalar);
    }
}