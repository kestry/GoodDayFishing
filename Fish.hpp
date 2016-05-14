#ifndef FISH_HPP
#define FISH_HPP

#include <string>
#include "Sprite.hpp"
#include "World.hpp"
#include <iostream>
using namespace std;
enum class SwallowSize{
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
    SwallowSize swallow_size;
    int points;

    FishSpecies() {}
    FishSpecies(std::string n
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
};

class Fish {
public:
    int head_x;
    int head_y;
    int x_velocity;
    int y_velocity;
    FishState state;
    const FishSpecies* species;

    Fish();
    Fish(const FishSpecies &species);
    
    void update();

    void revive();
    void capture();
    void kill();

    int width();
    
    void swapDirection();
    void draw(World &world);
    void killOffscreenFish();

private:
    int is_leftward_;
};
inline void Fish::swapDirection() { x_velocity = -x_velocity; }
inline int Fish::width() { return species->sprite_left.size(); }

#endif