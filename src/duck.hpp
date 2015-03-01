/*
 * Duck class declaration
 * Created by Casey Chesshir
 * February 22, 2015
 */

#ifndef DUCK_HPP
#define DUCK_HPP

class Duck{
private:
    int x;
    int y;
    int xspeed;
    int yspeed;
public:
    Duck(int x, int y);
    int get_x();
    int get_y();
    void set_x(int new_x);
    void set_y(int new_y);
    void set_xspeed(int new_x);
    void set_yspeed(int new_y);
    
    SDL_Surface *duckSurface;
    SDL_Texture *duckTexture;
    
    void jump();
    void renderDuck();
    void shootFootball();
    void renderFootball();
    
    Mix_Chunk *quack;
    
};


#endif