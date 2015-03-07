#include "beaver.hpp"
#include "mallard.hpp"


Beaver::Beaver(){
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