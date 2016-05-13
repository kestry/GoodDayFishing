#include <iostream>
#include "FishManager.hpp"
using namespace std;

FishManager::FishManager(World &world) 
    : inclusive_species_distribution_(0, FishManagerConstant::NUM_SPECIES - 1)
    , inclusive_base_y_distribution_(0, FishManagerConstant::FISH_Y_SPACING - 1) {
    fish_species_array_ = new FishSpecies[FishManagerConstant::NUM_SPECIES];
    fish_array_ = new Fish[FishManagerConstant::FISH_ARRAY_SIZE];
    loadSpecies();
    generateInitialFish(world);
}

FishManager::~FishManager() {
    delete[] fish_species_array_;
    delete[] fish_array_;
}

void FishManager::loadSpecies() {
    loadOneSpecies(0, "starfish", "*", "*", BodySize::starfish, 500);
    loadOneSpecies(1, "jellyfish", "Q", "Q", BodySize::starfish, -800);
    loadOneSpecies(2, "goldfish", "o<", ">o", BodySize::goldfish, 300);
    loadOneSpecies(3, "mackerel", "<><", "><>", BodySize::mackerel, 700);
    loadOneSpecies(4, "tuna", "<\")<", ">(\">", BodySize::tuna, 1000);
}

void FishManager::loadOneSpecies(int index, std::string name, std::string ascii_left, std::string ascii_right, BodySize body_size, int points) {
    fish_species_array_[index].name = name;
    fish_species_array_[index].sprite_left = ascii_left;
    fish_species_array_[index].sprite_right = ascii_right;
    fish_species_array_[index].body_size = body_size;
    fish_species_array_[index].points = points;
}

void FishManager::update(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::alive) {
            fish_array_[iFish].update();
            fish_array_[iFish].killIfOffscreen();
        }
        else {
            generateNewFish(iFish, world);
        }
    }
}

void FishManager::draw(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::alive) {
            fish_array_[iFish].draw(world);
        }
    }
}

//Prerequisite: iFish fish must be dead
//  reason: otherwise will change the caught or live fish
//  note: can generate over "caught" fish if hook copies fish traits
void FishManager::generateNewFish(int iFish, World &world) {
    //generate live or dead
    if (world.randomBool()) {
        fish_array_[iFish].state = FishState::alive;
        fish_array_[iFish].velocity = 1 - (world.randomBool() << 1);
        fish_array_[iFish].head_y = FishManagerConstant::STAGE_MIN_FISH_Y + randomBaseY(world) + iFish * FishManagerConstant::FISH_Y_SPACING;
        fish_array_[iFish].species = &randomSpecies(world);
        fish_array_[iFish].head_x = (fish_array_[iFish].velocity < 0) ?
            WorldConstant::STAGE_LAST_X : WorldConstant::STAGE_FIRST_X;
    }
}

void FishManager::generateInitialFish(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        generateNewFish(iFish, world);
        if (FishState::alive == fish_array_[iFish].state) {
            fish_array_[iFish].head_x = world.randomStageX();
            fish_array_[iFish].killIfOffscreen();
        }
    }
}
