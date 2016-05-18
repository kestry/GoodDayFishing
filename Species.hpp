//Species.hpp  -- header file for Species class
//updated 2016/05/11

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include "Sprite.hpp"

enum class SwallowSize{
    nothing,     //j
    starfish,    //*
    //jellyfish, //Q  //commented out to be same size as starfish
    goldfish,   //o<
    mackerel,   //<><
    tuna,       //<")<
};

const struct Species {
    std::string name;
    Sprite sprite_left;
    Sprite sprite_right;
    SwallowSize swallow_size;
    int points;

    Species() {}
    Species(std::string n
        , Sprite sl
        , Sprite sr
        , SwallowSize ss
        , int p);
};

#endif