//GameObjectManager.cpp  -- header file for GameObjectManager class
//updated 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include "GameObjectManager.hpp"
#include <iostream>
using namespace std;

GameObjectManager::GameObjectManager(World &world)
    : bird_animation_delay_(0)
    , numHooked_(0)
    , hook_home_x_(0)
    , hook_home_y_(0)
    , inclusive_bird_odds_distribution_(0, GameObjectManagerConstant::BIRD_SPAWN_ODDS_PER_UPDATE)
    , inclusive_poop_odds_distribution_(0, GameObjectManagerConstant::POOP_SPAWN_ODDS_PER_UPDATE)
    , inclusive_species_distribution_(SpeciesConstant::I_FIRST_FISH_SPECIES,
                                        SpeciesConstant::NUM_SPECIES - 1) //last =  size - 1
    , inclusive_base_y_distribution_(0, GameObjectManagerConstant::FISH_Y_SPACING - 1) { //last =  size - 1
    fish_species_array_ = new Species[SpeciesConstant::NUM_SPECIES];
    fish_array_ = new GameObject[GameObjectManagerConstant::GAME_OBJECT_ARRAY_SIZE];
    initialize();
    generateInitialFish(world);
}

GameObjectManager::~GameObjectManager() {
    delete[] fish_species_array_;
    delete[] fish_array_;
}

inline void GameObjectManager::initialize() {
    loadSpecies();

    bird_ = &fish_array_[GameObjectManagerConstant::I_BIRD];
    bird_->head_y = GameObjectManagerConstant::STAGE_BIRD_Y;
    bird_->species = &fish_species_array_[SpeciesConstant::I_BIRD_SPECIES];

    poop_ = &fish_array_[GameObjectManagerConstant::I_POOP];
    poop_->species = &fish_species_array_[SpeciesConstant::I_POOP_SPECIES];

    hook_ = &fish_array_[GameObjectManagerConstant::I_HOOK];
}

void GameObjectManager::loadSpecies() {
    loadOneSpecies(0, "poop", ".", ".", SwallowSize::nothing, 0);
    loadOneSpecies(1, "seagull", "v", "m", SwallowSize::nothing, 0);
    loadOneSpecies(2, "hook", "j", "j", SwallowSize::nothing, 0);
    loadOneSpecies(3, "starfish", "*", "*", SwallowSize::starfish, 300);
    loadOneSpecies(4, "jellyfish", "Q", "Q", SwallowSize::starfish, -200);
    loadOneSpecies(5, "goldfish", "o<", ">o", SwallowSize::goldfish, 100);
    loadOneSpecies(6, "mackerel", "<><", "><>", SwallowSize::mackerel, 400);
    loadOneSpecies(7, "tuna", "<\")<", ">(\">", SwallowSize::tuna, 600);
}

void GameObjectManager::loadOneSpecies(int index, std::string name, std::string ascii_left, std::string ascii_right, SwallowSize swallow_size, int points) {
    fish_species_array_[index].name = name;
    fish_species_array_[index].sprite_left = ascii_left;
    fish_species_array_[index].sprite_right = ascii_right;
    fish_species_array_[index].swallow_size = swallow_size;
    fish_species_array_[index].points = points;
}

void GameObjectManager::generateInitialFish(World &world) {
    for (int iFish = GameObjectManagerConstant::I_FIRST_FISH;
        iFish < GameObjectManagerConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        generateOneFish(iFish, world);
        if (GameObjectState::alive == fish_array_[iFish].state) {
            fish_array_[iFish].head_x = world.randomStageX();
        }
    }
}

//Prerequisite: iFish GameObject must be dead
//  reason: otherwise will change the caught or live GameObject
void GameObjectManager::generateOneFish(int iFish, World &world) {
    //generate live or dead
    if (world.randomBool()) {
        fish_array_[iFish].revive();
        if (world.randomBool()) {
            fish_array_[iFish].reverse();
        }
        //to calculate head_y, subtract 1 from index, since index 0 is reserved
        fish_array_[iFish].head_y = GameObjectManagerConstant::STAGE_MIN_FISH_Y + randomBaseY(world) +
            (iFish - GameObjectManagerConstant::I_FIRST_FISH) * GameObjectManagerConstant::FISH_Y_SPACING;
        fish_array_[iFish].species = &randomSpecies(world);
        setInitialX(fish_array_[iFish]);
    }
}

void GameObjectManager::setInitialX(GameObject& GameObject) {
    GameObject.head_x = (GameObject.isLeftward()) ?
        WorldConstant::STAGE_LAST_X - 1 : WorldConstant::STAGE_FIRST_X + 1;
}

void GameObjectManager::regenerate(World &world) {
    generateBird(world);
    generatePoop(world);
    generateFish(world);
}

void GameObjectManager::generateBird(World &world) {
    if (bird_->state == GameObjectState::dead && !randomBirdSpawn(world)) {
        bird_->revive();
        if (world.randomBool()) {
            bird_->reverse();
        }
        setInitialX(*bird_);
    }
}

void GameObjectManager::generatePoop(World &world) {
    if (bird_->state == GameObjectState::alive &&
        poop_->state == GameObjectState::dead && !randomPoopSpawn(world)) {
        poop_->sink();
        poop_->head_x = bird_->head_x;
        poop_->head_y = bird_->head_y;
    }
}

void GameObjectManager::generateFish(World &world) {
    int count = 0;
    for (int iFish = GameObjectManagerConstant::I_FIRST_FISH; count < GameObjectManagerConstant::MAX_NEW_FISH_PER_UPDATE &&
        iFish < GameObjectManagerConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == GameObjectState::dead) {
            generateOneFish(iFish, world);
            count += fish_array_[iFish].state == GameObjectState::alive;
        }
    }
}

void GameObjectManager::update() {
    animateBird();
    checkHookPosition();
    for (int iObject = 0; iObject < GameObjectManagerConstant::GAME_OBJECT_ARRAY_SIZE; iObject++) {
        fish_array_[iObject].update();
    }
}

void GameObjectManager::animateBird() {
    ++bird_animation_delay_;
    if (bird_animation_delay_ == GameObjectManagerConstant::BIRD_ANIMATION_DELAY) {
        bird_->animate();
        bird_animation_delay_ = 0;
    }
}

void GameObjectManager::checkHookPosition() {
    if (isReeling() && hook_->head_y <= hook_home_y_) {
        restHook();
    }
    else if (hook_->head_y >= WorldConstant::STAGE_LAST_Y) {
        reelHook();
    }
}

void GameObjectManager::castHook(int home_x, int home_y) {
    numHooked_ = 0;
    hook_home_x_ = home_x;
    hook_home_y_ = home_y;
    hook_->head_x = home_x;
    hook_->head_y = home_y;
    hook_->species = &fish_species_array_[SpeciesConstant::I_HOOK_SPECIES];
    hook_->sink();
}

void GameObjectManager::reelHook() { 
    hook_->reel(); 
    hook_->head_x = hook_home_x_;
}

//Private nonmember GameObject hash function
//purpose: gets the index of the GameObject that live in the vertical 
//  vicinity of the hook
//note: used in member function, isFishingCollision, below
int fishHash(int y) {
    return GameObjectManagerConstant::I_FIRST_FISH + 
        (y - GameObjectManagerConstant::STAGE_MIN_FISH_Y) / 
        GameObjectManagerConstant::FISH_Y_SPACING;
}

//Requirement: a caller must call this function to update catch  
//  Version 1 caller == Player
//Checks if GameObject is hooked and then catches the GameObject
//purpose: updates hook to new caught GameObject, and allows Player to retrieve points
//note: uses private nonmember function, fishHash, above
bool GameObjectManager::isFishingCollision() {
    if (hook_->state != GameObjectState::caught ||
        hook_->head_y < GameObjectManagerConstant::STAGE_MIN_FISH_Y) {
        return false;
    }
    GameObject* nearby_fish = &fish_array_[fishHash(hook_->head_y)];
    if (nearby_fish->state != GameObjectState::alive ||
        nearby_fish->head_y != hook_->head_y ||
        nearby_fish->species->swallow_size <= hook_->species->swallow_size ||
        !hasBittenLength(*nearby_fish)) {
        return false;
    }
    doCatch(nearby_fish);
    return true;
}

//Replaces current hook with the new hooked GameObject and updates count
void GameObjectManager::doCatch(GameObject* GameObject) {
    GameObject->kill();
    hook_->reel();
    hook_->head_x = hook_home_x_;
    if (GameObject->isLeftward() != hook_->isLeftward()) {
        hook_->reverse();
    }
    hook_->species = GameObject->species;
    ++numHooked_;
}

bool GameObjectManager::hasBittenLength(const GameObject& GameObject) const {
    int min_x = (hook_->isLeftward()) ? hook_->head_x : hook_->head_x - hook_->width() - 1;
    int max_x = (hook_->isLeftward()) ? hook_->head_x + hook_->width() - 1 : hook_->head_x;
    return (min_x <= GameObject.head_x) && (GameObject.head_x <= max_x);
}

void GameObjectManager::draw(World &world) {
    for (int iFish = GameObjectManagerConstant::I_FIRST_EARLY_DRAW; iFish < GameObjectManagerConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state > GameObjectState::dead) {
            fish_array_[iFish].draw(world);
        }
    }
    if (hook_->state == GameObjectState::caught) {
        for (int line_y = hook_home_y_; line_y < hook_->head_y; line_y++) {
            world.drawTile('|', hook_home_x_, line_y);
        }
    }
}

bool GameObjectManager::isPoopCollision(int x, int y) {
    if (poop_->state == GameObjectState::pending
        && x == poop_->head_x 
        && y == poop_->head_y) {
        return true;
    }
    return false;
}

void GameObjectManager::lateUpdate(World &world) {
    switch (poopTile(world)) {
    case ' ':
        break;
    case 'o':
        poop_->state = GameObjectState::pending;
        break;
    default:
        poop_->state = GameObjectState::pending;
        break;
    }
}

void GameObjectManager::lateDraw(World &world) {
    switch (poop_->state) {
    case GameObjectState::pending:
        poop_->kill();
        //fallthrough
    case GameObjectState::dead:
        break;
    default:
        poop_->draw(world);
        break;
    }
}