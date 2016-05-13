#ifndef FISH_HPP
#define FISH_HPP

#include <string>
#include "Sprite.hpp"
#include "World.hpp"

enum class BodySize {
    hook,       //j
    starfish,   //*
    //jellyfish,  //Q
    goldfish,   //o<
    mackerel,   //<><
    tuna,       //<")<
};

enum class FishState {
    dead,
    caught,
    alive
};

const struct FishSpecies {
    std::string name;
    Sprite sprite_left;
    Sprite sprite_right;
    BodySize body_size;
    int points;
};

class Fish {
public:
    int head_x;
    int head_y;
    int velocity;
    FishState state;
    const FishSpecies* species;

    Fish();
    Fish(const FishSpecies &species);
    
    int size();
    void update();
    
    void swapDirection();
    void draw(World &world);
    void killIfOffscreen();
};
inline void Fish::swapDirection() { velocity = -velocity; }
inline int Fish::size() { return species->sprite_left.size(); }

#endif