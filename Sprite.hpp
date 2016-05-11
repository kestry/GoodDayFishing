//Sprite.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#pragma once

#include <vector>
#include <string>
#include "World.hpp"

class Sprite {
public:
    Sprite(const std::string &fileName);

    //inline functions
    int     maxWidth() const;
    int     width(int y) const;
    int     height() const;
    char    tile(int x, int y) const;
    void    draw(World &world, int x0, int y0) const;  //draw sprite in world, starting from (x0, y0)

    bool    load(const std::string &fileName);
private:
    int max_width_;
    std::vector<std::string> sprite_;
};

inline int Sprite::maxWidth() const { return max_width_; }
inline int Sprite::width(int y) const { return sprite_[y].size(); }
inline int Sprite::height() const { return sprite_.size(); }
inline char Sprite::tile(int x, int y) const { return sprite_[y][x]; }