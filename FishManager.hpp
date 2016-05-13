#ifndef FISH_MANAGER_HPP
#define FISH_MANAGER_HPP

#include "Fish.hpp"
#include "Hook.hpp"
#include "Sprite.hpp"

namespace FishManagerConstant {
    //species hardcoded into FishManager::load()
    const int NUM_SPECIES = 5;

    //numbers are adjustable
    const int STAGE_MIN_FISH_Y = WorldConstant::WATER_Y + 1;
    const int FISH_Y_SPACING = 2;
    const int FISH_ARRAY_SIZE = (WorldConstant::STAGE_LAST_Y + 1 - STAGE_MIN_FISH_Y) / FISH_Y_SPACING;
}

class FishManager {
public:
    FishManager(World &);
    FishManager(const FishManager &) = delete;
    ~FishManager();
    FishManager &operator=(const FishManager &) = delete;

    void update(World &);
    void draw(World &);    


private:
    FishSpecies* fish_species_array_;
    Fish* fish_array_; 
    std::uniform_int_distribution<int> inclusive_species_distribution_;
    std::uniform_int_distribution<int> inclusive_base_y_distribution_;

    //init functions
    void loadSpecies();
    void loadOneSpecies(int index, std::string name,
        std::string ascii_left, std::string ascii_right, BodySize body_size, int poinsts);
    void generateInitialFish(World &);
    void generateNewFish(int, World &);

    const FishSpecies& randomSpecies(World &world) const;
    size_t randomBaseY(World &world);
};
inline const FishSpecies& FishManager::randomSpecies(World &world) const {
    return fish_species_array_[inclusive_species_distribution_(world.engine())];
}

inline size_t FishManager::randomBaseY(World &world) { return inclusive_base_y_distribution_(world.engine()); }



#endif