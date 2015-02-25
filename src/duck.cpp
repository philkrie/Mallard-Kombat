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

int Duck::get_x() const{
    return x;
}

int Duck::get_y() const{
    return y;
}

void Duck::set_y(int new_y){
    y = new_y;
    
    if(y < 0)
        y = 0;
}
/*
void Duck::yspeed(int new_y){
    y += new_y;
    
    if (y < 0)
        y = 0;
}

void Duck::xspeed(int new_x){
    x += new_x;
    if (x < 0)
        x = 0;
}

void Duck::jump(){
    this.yspeed = 20;
}

while(Duck.get_y() != 480){
    yspeed--;
}*/