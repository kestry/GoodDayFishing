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
    
    void update();
    

    void            setSpecies(const FishSpecies &species);
    void            swapDirection();
    void            draw(World &world);

private:
    //can have this as a single char bitmap
    bool fish_state_;

};
inline void            Fish::setSpecies(const FishSpecies &newSpecies) { species = &newSpecies; }
inline void            Fish::swapDirection() { velocity = -velocity; }

#endif