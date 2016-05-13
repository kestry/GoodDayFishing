#ifndef FISH_MANAGER_HPP
#define FISH_MANAGER_HPP

#include "Fish.hpp"
#include "Hook.hpp"
#include "Sprite.hpp"

namespace FishManagerConstant {
    //species hardcoded into FishManager::load()
    const int NUM_SPECIES = 5;

    //numbers are adjustable
    const int FISH_ARRAY_SIZE = 7;
    const int STAGE_MIN_FISH_Y = WorldConstant::WATER_Y + 2;
}

class FishManager {
public:
    FishManager(World &);
    FishManager(const FishManager &) = delete;
    ~FishManager();
    FishManager &operator=(const FishManager &) = delete;

    void update();
    void draw(World &world);    


private:
    FishSpecies* fish_species_array_;
    Fish* fish_array_; 
    std::uniform_int_distribution<int> inclusive_species_distribution_;
    std::uniform_int_distribution<int> inclusive_base_fish_y_distribution_;

    //init functions
    void loadSpecies();
    void loadOneSpecies(int index, std::string name,
        std::string ascii_left, std::string ascii_right, BodySize body_size, int poinsts);
    void generateFish(World &);
    void generateOneFish(World &);

};

#endif