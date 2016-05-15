#include "FishManager.hpp"
#include <iostream>
using namespace std;
FishManager::FishManager(World &world)
    : bird_animation_delay_(0)
    , numHooked_(0)
    , hook_home_x_(0)
    , hook_home_y_(0)
    , inclusive_bird_odds_distribution_(0, GameObjectConstant::BIRD_SPAWN_ODDS_PER_UPDATE)
    , inclusive_poop_odds_distribution_(0, GameObjectConstant::POOP_SPAWN_ODDS_PER_UPDATE)
    , inclusive_species_distribution_(SpeciesConstant::I_FIRST_FISH_SPECIES,
                                        SpeciesConstant::NUM_SPECIES - 1) //last =  size - 1
    , inclusive_base_y_distribution_(0, GameObjectConstant::FISH_Y_SPACING - 1) { //last =  size - 1
    fish_species_array_ = new Species[SpeciesConstant::NUM_SPECIES];
    fish_array_ = new Fish[GameObjectConstant::GAME_OBJECT_ARRAY_SIZE];
    initialize();
    generateInitialFish(world);
}

FishManager::~FishManager() {
    delete[] fish_species_array_;
    delete[] fish_array_;
}

inline void FishManager::initialize() {
    loadSpecies();

    bird_ = &fish_array_[GameObjectConstant::I_BIRD];
    bird_->head_y = GameObjectConstant::STAGE_BIRD_Y;
    bird_->species = &fish_species_array_[SpeciesConstant::I_BIRD_SPECIES];

    poop_ = &fish_array_[GameObjectConstant::I_POOP];
    poop_->species = &fish_species_array_[SpeciesConstant::I_POOP_SPECIES];

    hook_ = &fish_array_[GameObjectConstant::I_HOOK];
}

void FishManager::loadSpecies() {
    loadOneSpecies(0, "seagull", "v", "m", SwallowSize::nothing, 0);
    loadOneSpecies(1, "poop", ".", ".", SwallowSize::nothing, 0);
    loadOneSpecies(2, "hook", "j", "j", SwallowSize::nothing, 0);
    loadOneSpecies(3, "starfish", "*", "*", SwallowSize::starfish, 500);
    loadOneSpecies(4, "jellyfish", "Q", "Q", SwallowSize::starfish, -800);
    loadOneSpecies(5, "goldfish", "o<", ">o", SwallowSize::goldfish, 300);
    loadOneSpecies(6, "mackerel", "<><", "><>", SwallowSize::mackerel, 700);
    loadOneSpecies(7, "tuna", "<\")<", ">(\">", SwallowSize::tuna, 1000);
}

void FishManager::loadOneSpecies(int index, std::string name, std::string ascii_left, std::string ascii_right, SwallowSize swallow_size, int points) {
    fish_species_array_[index].name = name;
    fish_species_array_[index].sprite_left = ascii_left;
    fish_species_array_[index].sprite_right = ascii_right;
    fish_species_array_[index].swallow_size = swallow_size;
    fish_species_array_[index].points = points;
}

void FishManager::generateInitialFish(World &world) {
    for (int iFish = GameObjectConstant::I_FIRST_FISH;
        iFish < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        generateOneFish(iFish, world);
        if (FishState::alive == fish_array_[iFish].state) {
            fish_array_[iFish].head_x = world.randomStageX();
        }
    }
}

//Prerequisite: iFish fish must be dead
//  reason: otherwise will change the caught or live fish
void FishManager::generateOneFish(int iFish, World &world) {
    //generate live or dead
    if (world.randomBool()) {
        fish_array_[iFish].revive();
        if (world.randomBool()) {
            fish_array_[iFish].reverse();
        }
        //to calculate head_y, subtract 1 from index, since index 0 is reserved
        fish_array_[iFish].head_y = GameObjectConstant::STAGE_MIN_FISH_Y + randomBaseY(world) +
            (iFish - GameObjectConstant::I_FIRST_FISH) * GameObjectConstant::FISH_Y_SPACING;
        fish_array_[iFish].species = &randomSpecies(world);
        setInitialX(fish_array_[iFish]);
    }
}

void FishManager::setInitialX(Fish& fish) {
    fish.head_x = (fish.isLeftward()) ?
        WorldConstant::STAGE_LAST_X - 1 : WorldConstant::STAGE_FIRST_X + 1;
}

void FishManager::regenerate(World &world) {
    generateBird(world);
    generatePoop(world);
    generateFish(world);
}

void FishManager::generateBird(World &world) {
    if (bird_->state == FishState::dead && !randomBirdSpawn(world)) {
        bird_->revive();
        if (world.randomBool()) {
            bird_->reverse();
        }
        setInitialX(*bird_);
    }
}

void FishManager::generatePoop(World &world) {
    if (bird_->state == FishState::alive &&
        poop_->state == FishState::dead && !randomPoopSpawn(world)) {
        poop_->sink();
        poop_->head_x = bird_->head_x;
        poop_->head_y = bird_->head_y + 1;
    }
}

void FishManager::generateFish(World &world) {
    int count = 0;
    for (int iFish = GameObjectConstant::I_FIRST_FISH; count < GameObjectConstant::MAX_NEW_FISH_PER_UPDATE &&
        iFish < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::dead) {
            generateOneFish(iFish, world);
            count += fish_array_[iFish].state == FishState::alive;
        }
    }
}

void FishManager::update() {
    animateBird();
    checkHookPosition();
    for (int iObject = 0; iObject < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iObject++) {
        fish_array_[iObject].update();
    }
}

void FishManager::animateBird() {
    ++bird_animation_delay_;
    if (bird_animation_delay_ == GameObjectConstant::BIRD_ANIMATION_DELAY) {
        bird_->animate();
        bird_animation_delay_ = 0;
    }
}

void FishManager::checkHookPosition() {
    if (isReeling() && hook_->head_y <= hook_home_y_) {
        restHook();
    }
    else if (hook_->head_y >= WorldConstant::STAGE_LAST_Y) {
        reelHook();
    }
}

void FishManager::castHook(int home_x, int home_y) {
    numHooked_ = 0;
    hook_home_x_ = home_x;
    hook_home_y_ = home_y;
    hook_->head_x = home_x;
    hook_->head_y = home_y;
    hook_->species = &fish_species_array_[SpeciesConstant::I_HOOK_SPECIES];
    hook_->sink();
}

void FishManager::reelHook() { 
    hook_->reel(); 
    hook_->head_x = hook_home_x_;
}

//Private nonmember fish hash function
//purpose: gets the index of the fish that live in the vertical 
//  vicinity of the hook
//note: used in member function, isFishingCollision, below
int fishHash(int y) {
    return GameObjectConstant::I_FIRST_FISH + 
        (y - GameObjectConstant::STAGE_MIN_FISH_Y) / 
        GameObjectConstant::FISH_Y_SPACING;
}

//Checks if fish is hooked and then catches the fish
//purpose: updates hook to new caught fish, and allows Player to retrieve points
//note: uses private nonmember function, fishHash, above
bool FishManager::isFishingCollision() {
    if (hook_->state != FishState::caught ||
        hook_->head_y < GameObjectConstant::STAGE_MIN_FISH_Y) {
        return false;
    }
    Fish* nearby_fish = &fish_array_[fishHash(hook_->head_y)];
    if (nearby_fish->state != FishState::alive ||
        nearby_fish->head_y != hook_->head_y ||
        nearby_fish->species->swallow_size <= hook_->species->swallow_size ||
        !hasBittenLength(*nearby_fish)) {
        return false;
    }
    doCatch(nearby_fish);
    return true;
}

//Replaces current hook with the new hooked fish and updates count
void FishManager::doCatch(Fish* fish) {
    fish->kill();
    hook_->reel();
    hook_->head_x = hook_home_x_;
    if (fish->isLeftward() != hook_->isLeftward()) {
        hook_->reverse();
    }
    hook_->species = fish->species;
    ++numHooked_;
}

bool FishManager::hasBittenLength(const Fish& fish) const {
    int min_x = (hook_->isLeftward()) ? hook_->head_x : hook_->head_x - hook_->width() - 1;
    int max_x = (hook_->isLeftward()) ? hook_->head_x + hook_->width() - 1 : hook_->head_x;
    return (min_x <= fish.head_x) && (fish.head_x <= max_x);
}

void FishManager::draw(World &world) {
    for (int iFish = 0; iFish < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state != FishState::dead) {
            fish_array_[iFish].draw(world);
        }
    }
    if (hook_->state == FishState::caught) {
        for (int line_y = hook_home_y_; line_y < hook_->head_y; line_y++) {
            world.drawTile('|', hook_home_x_, line_y);
        }
    }
}