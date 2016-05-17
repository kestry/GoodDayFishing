#ifndef GameObject_MANAGER_HPP
#define GameObject_MANAGER_HPP

#include "GameObject.hpp"
#include "Sprite.hpp"

class Player;
class Hook;

namespace SpeciesConstant {
    //NOTE: GameObject are the last segment for both species and game objects
    //=======================================
    //species hardcoded into GameObjectManager::load()
    const int NUM_POOP_SPECIES = 1;
    const int NUM_BIRD_SPECIES = 1;
    const int NUM_HOOK_SPECIES = 1;
    const int NUM_FISH_SPECIES = 5;
    const int NUM_SPECIES = NUM_POOP_SPECIES + NUM_BIRD_SPECIES +
                NUM_HOOK_SPECIES + NUM_FISH_SPECIES;
    const int I_POOP_SPECIES = 0;
    const int I_BIRD_SPECIES = 1;
    const int I_HOOK_SPECIES = 2;
    const int I_FIRST_FISH_SPECIES = 3;
}

namespace GameObjectManagerConstant {
    //=======================================
    //Adjustable Y zones
    const int STAGE_BIRD_Y = WorldConstant::STAGE_FIRST_Y + 1;
    const int STAGE_MIN_FISH_Y = WorldConstant::WATER_Y + 1;
    const int FISH_Y_SPACING = 2;

    //Number of Game Objects (in array)
    //(Note: must adjust index constants below)
    const int NUM_POOP = 1;
    const int NUM_BIRD = 1;
    const int NUM_HOOK = 1;
    //note: @see NUM_FISH below

    //Game Object Indices 
    //(Note: must be in relation to above)
    const int I_FIRST_EARLY_DRAW = 1;
    const int I_POOP = 0;
    const int I_BIRD = 1;
    const int I_HOOK = 2;
    const int I_FIRST_FISH = 3;

    //Self-adjusting values that are dependent on above
    const int FISH_ZONE_HEIGHT = WorldConstant::STAGE_LAST_Y + 1 - STAGE_MIN_FISH_Y;
    const int NUM_FISH = FISH_ZONE_HEIGHT / FISH_Y_SPACING;
    const int GAME_OBJECT_ARRAY_SIZE = NUM_BIRD + NUM_POOP + NUM_HOOK + NUM_FISH;

    //=======================================
    //Adjustable Quality of Life Constants
    const int MAX_NEW_FISH_PER_UPDATE = 2;
    const int BIRD_SPAWN_ODDS_PER_UPDATE = 20; //higher = lower chance; (1 in ODDS chance)
    const int POOP_SPAWN_ODDS_PER_UPDATE = 1; //higher = lower chance; (1 in ODDS chance)
    const int BIRD_ANIMATION_DELAY = 2;
}

class GameObjectManager {
public:
    GameObjectManager(World &);
    GameObjectManager(const GameObjectManager &) = delete;
    ~GameObjectManager();
    GameObjectManager     &operator=(const GameObjectManager &) = delete;

    void            update();
    void            regenerate(World &);
    void            draw(World &);
    void            lateUpdate(World &);
    void            lateDraw(World &);

    void            castHook(int home_x, int home_y);
    void            reelHook();                         //inline
    bool            isFishing();                        //inline
    bool            isFishingCollision();
    bool            isPoopCollision(int, int);
    int             points() const;                     //inline

private:
    int bird_animation_delay_;
    int numHooked_;                 //used as a multiplier in points()
    int hook_home_x_;
    int hook_home_y_;
    Species* fish_species_array_;
    GameObject* fish_array_;
    GameObject* bird_;
    GameObject* poop_;
    GameObject* hook_;
    std::uniform_int_distribution<int> inclusive_bird_odds_distribution_;
    std::uniform_int_distribution<int> inclusive_poop_odds_distribution_;
    std::uniform_int_distribution<int> inclusive_species_distribution_;
    std::uniform_int_distribution<int> inclusive_base_y_distribution_;

    //init functions
    void            initialize();
    void            loadSpecies();
    void            loadOneSpecies(int, std::string, std::string, std::string, SwallowSize, int);
    void            generateInitialFish(World &);

    //generators
    void            generateBird(World &);
    void            generatePoop(World &);
    void            generateFish(World &);
    void            generateOneFish(int, World &);
    void            setInitialX(GameObject&);      //sets at initial spawn x of GameObject

    //random number generators
    const Species&  randomSpecies(World &world) const;  //inline
    size_t          randomBaseY(World &world);          //inline
    size_t          randomBirdSpawn(World &world);      //inline
    size_t          randomPoopSpawn(World &world);      //inline

    //update helper functions
    void            animateBird();
    bool            isReeling();                        //inline
    void            restHook();                         //inline
    void            checkHookPosition();

    //collision helper functions
    bool            hasBittenLength(const GameObject&) const;
    void            doCatch(GameObject* GameObject);

    //late update functions (aka after initial draw())
    void            checkPoopPosition();
    char            poopTile(World &world);



};
inline int GameObjectManager::points() const { return numHooked_ * hook_->species->points; }

inline void GameObjectManager::restHook() { hook_->kill(); }

inline bool GameObjectManager::isFishing() { return hook_->state != GameObjectState::dead; }
inline bool GameObjectManager::isReeling() { return hook_->isUpward(); }


inline const Species& GameObjectManager::randomSpecies(World &world) const {
    return fish_species_array_[inclusive_species_distribution_(world.engine())];
}
inline size_t GameObjectManager::randomBaseY(World &world) { return inclusive_base_y_distribution_(world.engine()); }
inline size_t GameObjectManager::randomBirdSpawn(World &world) { return inclusive_bird_odds_distribution_(world.engine()); }
inline size_t GameObjectManager::randomPoopSpawn(World &world) { return inclusive_poop_odds_distribution_(world.engine()); }

inline char GameObjectManager::poopTile(World &world) { return world.tile(poop_->head_x, poop_->head_y); }

#endif