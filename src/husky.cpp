//
//  husky.cpp
//  
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#include "husky.hpp"
#include "mallard.hpp"

Husky::Husky(int x, int y){
    enemyScalar.x = x;
    enemyScalar.y = y;
    hasFootball = false;
}

void Husky::respawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50 * Mallard::ycor,Mallard::SCREEN_HEIGHT - 50);
    enemyScalar.y = distribution(generator);
}

void Husky::render(SDL_Renderer *ren){
    SDL_RenderCopy(ren, enemyTexture, NULL, &enemyScalar);
}

void Husky::move(){
    enemyScalar.x -= 2;
    if (rand()%10 < 5)
    	enemyScalar.y += rand()%10;
    else
    	enemyScalar.y -= rand()%10;
}
