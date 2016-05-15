#ifndef FISH_HPP
#define FISH_HPP

#include <string>
#include "Sprite.hpp"
#include "World.hpp"
#include <iostream>
using namespace std;
enum class SwallowSize{
    nothing,       //j
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
    FishState state;
    const FishSpecies* species;

    Fish();
    Fish(const FishSpecies &species);
    
    void update();
    void draw(World &world);

    void revive();
    void reverse();
    void sink();
    void reel();
    void kill();

    int width() const;
    bool isLeftward() const;
    bool isUpward() const;
private:
    bool is_leftward_;
    int x_velocity_;
    int y_velocity_;

    void killOffscreenFish();
};

inline int Fish::width() const { return species->sprite_left.size(); }
inline bool Fish::isLeftward() const { return is_leftward_; }
inline bool Fish::isUpward() const { return -1 == y_velocity_; }

#endif