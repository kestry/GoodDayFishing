#ifndef FISH_MANAGER_HPP
#define FISH_MANAGER_HPP

#include "Fish.hpp"
#include "Sprite.hpp"

class Player;
class Hook;

namespace SpeciesConstant {
    //NOTE: Fish are the last segment for both species and game objects
    //=======================================
    //species hardcoded into FishManager::load()
    const int NUM_BIRD_SPECIES = 1;
    const int NUM_POOP_SPECIES = 1;
    const int NUM_HOOK_SPECIES = 1;
    const int NUM_FISH_SPECIES = 5;
    const int NUM_SPECIES = NUM_BIRD_SPECIES + NUM_POOP_SPECIES +
        NUM_HOOK_SPECIES + NUM_FISH_SPECIES;
    const int I_BIRD_SPECIES = 0;
    const int I_POOP_SPECIES = 1;
    const int I_HOOK_SPECIES = 2;
    const int I_FIRST_FISH_SPECIES = 3;
}

//namespace GameObjectConstant {
namespace GameObjectConstant {
    //=======================================
    //Adjustable Y zones
    const int STAGE_BIRD_Y = WorldConstant::STAGE_FIRST_Y + 1;
    const int STAGE_MIN_FISH_Y = WorldConstant::WATER_Y + 1;
    const int FISH_Y_SPACING = 1;

    //Number of Game Objects (in array)
    //(Note: must adjust index constants below)
    const int NUM_BIRD = 1;
    const int NUM_POOP = 1;
    const int NUM_HOOK = 1;
    //note: @see NUM_FISH below

    //Game Object Indices 
    //(Note: must be in relation to above)
    const int I_BIRD = 0;
    const int I_POOP = 1;
    const int I_HOOK = 2;
    const int I_FIRST_FISH = 3;

    //Self-adjusting values that are dependent on above
    const int FISH_ZONE_HEIGHT = WorldConstant::STAGE_LAST_Y + 1 - STAGE_MIN_FISH_Y;
    const int NUM_FISH = FISH_ZONE_HEIGHT / FISH_Y_SPACING;
    const int GAME_OBJECT_ARRAY_SIZE = NUM_BIRD + NUM_POOP + NUM_HOOK + NUM_FISH;

    //=======================================
    //Adjustable Quality of Life Constants
    const int MAX_NEW_FISH_PER_UPDATE = 7;
}

class FishManager {
public:
    FishManager(World &);
    FishManager(const FishManager &) = delete;
    ~FishManager();
    FishManager &operator=(const FishManager &) = delete;

    void update();
    void generateBird(World &);
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
    Species* fish_species_array_;
    Fish* fish_array_;
    Fish* hook_;
    std::uniform_int_distribution<int> inclusive_species_distribution_;
    std::uniform_int_distribution<int> inclusive_base_y_distribution_;

    //inline private functions
    void restHook();
    const Species& randomSpecies(World &world) const;
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

inline const Species& FishManager::randomSpecies(World &world) const {
    return fish_species_array_[inclusive_species_distribution_(world.engine())];
}
inline size_t FishManager::randomBaseY(World &world) { return inclusive_base_y_distribution_(world.engine()); }
#endif