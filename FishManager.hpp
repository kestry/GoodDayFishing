#ifndef FISH_MANAGER_HPP
#define FISH_MANAGER_HPP

#include "Fish.hpp"
#include "Sprite.hpp"

class Player;
class Hook;

namespace FishManagerConstant {
    //species hardcoded into FishManager::load()
    const int NUM_SPECIES = 6;

    //numbers are adjustable
    const int STAGE_MIN_FISH_Y = WorldConstant::WATER_Y + 1;
    const int FISH_Y_SPACING = 1;
    const int MAX_NEW_FISH_PER_UPDATE = 7;
    const int FISH_ARRAY_SIZE = (WorldConstant::STAGE_LAST_Y + 1 - STAGE_MIN_FISH_Y) / FISH_Y_SPACING;
    const int I_HOOK = 0;
    const int I_FIRST_FISH = 1;
    const int I_FIRST_FISH_SPECIES = 1;

}

class FishManager {
public:
    FishManager(World &);
    FishManager(const FishManager &) = delete;
    ~FishManager();
    FishManager &operator=(const FishManager &) = delete;

    void update();
    void generateFish(World &);
    void draw(World &);

    void castHook(int home_x, int home_y);
    void reelHook();
    
    bool isFishing();
    bool isFishingCollision();
    int points() const;

private:
    int numHooked_;
    int home_x_;
    int home_y_;
    FishSpecies* fish_species_array_;
    Fish* fish_array_;
    Fish* hook_;
    std::uniform_int_distribution<int> inclusive_species_distribution_;
    std::uniform_int_distribution<int> inclusive_base_y_distribution_;

    //inline private functions
    void restHook();
    const FishSpecies& randomSpecies(World &world) const;
    size_t randomBaseY(World &world);

    //init functions
    void loadSpecies();
    void loadOneSpecies(int index, std::string name,
        std::string ascii_left, std::string ascii_right, SwallowSize swallow_size, int poinsts);
    void generateInitialFish(World &);
    
    //generators
    void generateOneFish(int, World &);

    //update  functions
    void checkHookPosition();
    bool isReeling();

    //collision helper functions
    bool hasBittenLength(const Fish&) const;
    void doCatch(Fish* fish);
};
inline bool FishManager::isReeling() { return hook_->isUpward(); }
inline bool FishManager::isFishing() { return hook_->state != FishState::dead; }


inline void FishManager::reelHook() { hook_->reel(); }
inline void FishManager::restHook() { hook_->kill(); }
inline int FishManager::points() const { return numHooked_ * hook_->species->points; }

inline const FishSpecies& FishManager::randomSpecies(World &world) const {
    return fish_species_array_[inclusive_species_distribution_(world.engine())];
}
inline size_t FishManager::randomBaseY(World &world) { return inclusive_base_y_distribution_(world.engine()); }
#endif