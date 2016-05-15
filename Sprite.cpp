//Sprite.cpp  -- implementation file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#include <iostream>
#include <fstream>
#include <sstream>
#include "Sprite.hpp"
using namespace std;

Sprite::Sprite() {

}

Sprite::Sprite(char c) {
    sprite_.push_back(string(1, c));
}

Sprite::Sprite(const std::string &eitherFilenameOrAscii) 
    : max_width_(0) {
    //
    if (eitherFilenameOrAscii.size() >= 4 &&
        eitherFilenameOrAscii.substr(eitherFilenameOrAscii.size() - 4, 4) == SpriteConstant::FILE_EXTENSION) {
        load(eitherFilenameOrAscii);
    }
    else { //else is sprite
        loadLine(eitherFilenameOrAscii);
    }
}

bool Sprite::load(const std::string &name) {
    ifstream file;
    file.open(name);

    if (file) {
        string line;
        while (getline(file, line)) {
            loadLine(line);
        }
    }
    else {
        return false;
    }
    return true;
}

void Sprite::loadLine(const std::string line) {
    max_width_ = line.size() > max_width_ ? line.size() : max_width_;
    sprite_.push_back(line);
}

bool Sprite::draw(World &world, int stage_x0, int stage_y0) const {
    bool isFullDraw = true;
    int sprite_first_x = 0;
    int sprite_max_last_x = max_width_ - 1;
    //partial drawing:
    //  start_x = 0 if stage_x0 > STAGE_FIRST_X, else
    //          = stage_first_x - stage_x0
    //   last_x = sprite_[y].size() - 1 if sprite_[y].size() - 1 + stage_x0 < STAGE_LAST_X, else
    //          = stage_first_x - stage_x0
    //truncated front partial draw
    if (stage_x0 < WorldConstant::STAGE_FIRST_X) {
        isFullDraw = false;
        sprite_first_x = WorldConstant::STAGE_FIRST_X - stage_x0;
    }
    //truncated rear partial draw
    //0 1 2 3 4 5 6
    //  x0
    //      wn
    //        xn    //size = 4
    //
    if (sprite_max_last_x + stage_x0 > WorldConstant::STAGE_LAST_X) {
        isFullDraw = false;
        sprite_max_last_x = sprite_max_last_x - (sprite_max_last_x + stage_x0 - WorldConstant::STAGE_LAST_X);
    }
    for (int y = 0; y < sprite_.size(); y++) {
        int sprite_last_x = (sprite_[y].size() - 1 < sprite_max_last_x) ? sprite_[y].size() - 1 : sprite_max_last_x;
        for (int x = sprite_first_x; x <= sprite_last_x; x++) {
            char ch = sprite_[y][x];
            switch (ch) {
            case SpriteConstant::TRANSPARENT_CHAR:
            case SpriteConstant::TRANSPARENT_CHAR2:
                break;
            case SpriteConstant::SOLID_WHITE_CHAR:
                ch = ' ';
                //fallthrough
            default:
                world.drawTile(ch, x + stage_x0, y + stage_y0);
                break;
            }
        }
    }
    return isFullDraw;
}

void Sprite::print() const {
    for (int y = 0; y < sprite_.size(); y++) {
        cout << sprite_[y] << endl;
    }
}