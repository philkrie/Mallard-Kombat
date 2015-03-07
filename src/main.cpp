#include "mallard.hpp"
#include <iostream>
int main(int argc, char *argv[]) {

    Mallard mallard(argc, argv);
    mallard.execute();
    std::cout << "wow" << std::endl;
    return 0;

}