//Sprite.cpp  -- implementation file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#include <iostream>
#include <fstream>
#include "Constants.hpp"
#include "Sprite.hpp"

using namespace std;

Sprite::Sprite(const std::string &name) 
    : max_width_(0) {
    load(name);
}

bool Sprite::load(const std::string &name) {
    ifstream file;
    file.open("assets//" + name + ".txt");
    if (file) {
        string line;
        while (getline(file, line)) {
            max_width_ = line.size() > max_width_ ? line.size() : max_width_;
            sprite_.push_back(line);
        }
    }
    else {
        return false;
    }
    return true;
}

//requirement: draw will be within bounds
//future: if want partial draws
//  option 1. implement check here
//  option 2. allow passing of end limit (aka complete range)
void Sprite::draw(World &world, int x0, int y0) const {
    for (int y = 0; y < sprite_.size(); y++) {
        for (int x = 0; x < sprite_[y].size(); x++) {
            char ch = sprite_[y][x];
            switch (ch) {
            case SpriteConstant::TRANSPARENT_CHAR:
                break;
            case SpriteConstant::SOLID_WHITE_CHAR:
                ch = ' ';
                //fallthrough
            default:
                world.drawTile(ch, x + x0, y + y0);
                break;
            }
        }
    }
}
