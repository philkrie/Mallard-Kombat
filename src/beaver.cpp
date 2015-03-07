#include "beaver.hpp"

Beaver::Beaver(){
    beaverSurface = SDL_LoadBMP("resources/images/beaver.bmp");
    beaverSurface = SDL_ConvertSurfaceFormat(beaverSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    beaverTexture = SDL_CreateTextureFromSurface(Mallard::renderer, beaverSurface);
    beaverVisible = true;
    beaverScalar.x = 500;
    beaverScalar.y = 370;
    beaverCount = 0;
    spawnPoint = 240;
}

void Beaver::respawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50,430);
    spawnPoint = distribution(generator);
}