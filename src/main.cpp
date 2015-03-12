#include "mallard.hpp"
#include <iostream>

//Simple main, executes the game
int main(int argc, char *argv[]) {

    Mallard mallard(argc, argv);
    mallard.execute();
    return 0;

}