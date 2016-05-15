#include "Species.hpp"

Species::Species(std::string n
    , Sprite sl
    , Sprite sr
    , SwallowSize ss
    , int p) {
    name = n;
    sprite_left = sl;
    sprite_right = sr;
    swallow_size = ss;
    points = p;
}