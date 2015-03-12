//
//  husky.cpp
//  
//
//  Created by Casey Chesshir on 3/8/15.
//
//

#include "trojan.hpp"
#include "mallard.hpp"

//Trojan is slow and large
Trojan::Trojan(int x, int y){
    enemyScalar.x = x;
    enemyScalar.y = y;
    hasFootball = false;
}

void Trojan::respawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50,Mallard::SCREEN_HEIGHT - 50);
    enemyScalar.y = distribution(generator);
}

void Trojan::render(SDL_Renderer *ren){
    SDL_RenderCopy(ren, enemyTexture, NULL, &enemyScalar);
}

void Trojan::move(){
    enemyScalar.x -= 1;
}
