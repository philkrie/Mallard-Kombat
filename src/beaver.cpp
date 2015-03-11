/*
 * Duck class definitions
 * Created by Casey Chesshir
 * March 7, 2015
 */
#include "beaver.hpp"
#include "mallard.hpp"

Beaver::Beaver(int x, int y){
    enemyScalar.x = x;
    enemyScalar.y = y;
    hasFootball = false;
    midline = y;
}

void Beaver::respawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50 * Mallard::ycor,Mallard::SCREEN_HEIGHT - 50);
    enemyScalar.y= distribution(generator);
    midline = enemyScalar.y;
}

void Beaver::render(SDL_Renderer *ren){
    SDL_RenderCopy(ren, enemyTexture, NULL, &enemyScalar);

    if (hasFootball) {
        SDL_RenderCopy(ren, footballTexture, NULL, &footballScalar);
    }

}

void Beaver::move(){
    enemyScalar.y = midline + 50 * Mallard::ycor *  sin(enemyScalar.x * PI/30);
    enemyScalar.x -= 1;
}