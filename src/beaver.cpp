/*
 * Duck class definitions
 * Created by Casey Chesshir
 * March 7, 2015
 */
#include "beaver.hpp"

Beaver::Beaver(int x, int y){
    beaverScalar.x = x;
    beaverScalar.y = y;
    spawnPoint = y;
    beaverVisible = true;
}

void Beaver::respawn(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(50,430);
    spawnPoint = distribution(generator);
}