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
    int get_x() const;
    int get_y() const;
    void set_x(int new_x);
    void set_y(int new_y);
    void set_x_speed(int new_x);
    void set_y_speed(int new_y);
    void jump();
};


#endif