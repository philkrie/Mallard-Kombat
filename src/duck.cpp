/*
 * Duck class definitions
 * Created by Casey Chesshir
 * February 22, 2015
 */

#include "duck.hpp"
#include "mallard.hpp"

Duck::Duck(int new_x, int new_y){
    x = new_x;
    y = new_y;
    
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

void Duck::set_yspeed(int new_y){
    y += new_y;
    
    if (y < 0)
        y = 0;
}

void Duck::set_xspeed(int new_x){
    x += new_x;
    if (x < 0)
        x = 0;
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

void Duck::renderDuck(){
    if (footballVisible) {
        footballScalar.x +=10;
        SDL_RenderCopy(renderer, footballTexture, NULL, &footballScalar);
        SDL_RenderCopy(renderer, DST[1], NULL, &duckScalar);
        if (footballScalar.x > 640 || didCollide(footballScalar, beaverScalar)){
            footballVisible = false;
        }
    }
    else if (isDuckDead) {
        SDL_RenderCopy(renderer, DST[3], NULL, &duckScalar);
    }
    
    else if (count <= 15){
        SDL_RenderCopy(renderer, DST[2], NULL, &duckScalar);
    }
    else if (count < 30){
        SDL_RenderCopy(renderer, DST[0], NULL, &duckScalar);
    }
}