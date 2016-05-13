#include <iostream>
#include "FishManager.hpp"
using namespace std;

FishManager::FishManager(World &world) 
    : inclusive_species_distribution_(0, FishManagerConstant::NUM_SPECIES - 1)
    , inclusive_base_fish_y_distribution_(0, FishManagerConstant::FISH_ARRAY_SIZE - 1) {
    fish_species_array_ = new FishSpecies[FishManagerConstant::NUM_SPECIES];
    fish_array_ = new Fish[FishManagerConstant::FISH_ARRAY_SIZE];
    loadSpecies();
    generateFish(world);
    {
        for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE - 1; iFish++) {
            cout << fish_array_[iFish].head_x << ", " << fish_array_[iFish].head_y << endl;
            fish_array_[iFish].species->sprite_left.print();
            fish_array_[iFish].species->sprite_right.print();
            cout << fish_array_[iFish].species->sprite_right.maxWidth() << endl;

        }
    }
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

void FishManager::generateFish(World &world) {
    int iFish = 0;
    int minY = FishManagerConstant::STAGE_MIN_FISH_Y;
    int increment = (WorldConstant::STAGE_LAST_Y - FishManagerConstant::STAGE_MIN_FISH_Y) / FishManagerConstant::FISH_ARRAY_SIZE;
    for (; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++, minY += increment) {
        //generate species
        fish_array_[iFish].species = &fish_species_array_[inclusive_species_distribution_(world.engine())];
        //generate direction
        fish_array_[iFish].velocity = 1 - (world.randomBool() << 1);
        //generate initial x position
        fish_array_[iFish].head_x = world.randomStageX();
        
        //flip fish, since no partial drawing
        //if leftward  and x > STAGE_LAST_X  - 1 - fish.maxWidth <"@<, flip fish
        //if rightward and x < STAGE_FIRST_X - 1 - fish.maxWidth >@">, flip fish

        fish_array_[iFish].head_x -= (fish_array_[iFish].velocity > 0) * (fish_array_[iFish].species->sprite_right.maxWidth() - 1);
        fish_array_[iFish].head_y = minY + inclusive_base_fish_y_distribution_(world.engine());
    }
    std::uniform_int_distribution<int> inclusive_last_fish_y_distribution_(minY - increment, WorldConstant::STAGE_LAST_Y);
    fish_array_[FishManagerConstant::FISH_ARRAY_SIZE - 1].head_y = +inclusive_last_fish_y_distribution_(world.engine());
}

void FishManager::generateOneFish(World &world) {
    int iFish = 0;
    int minY = FishManagerConstant::STAGE_MIN_FISH_Y;
    int increment = (WorldConstant::STAGE_LAST_Y - FishManagerConstant::STAGE_MIN_FISH_Y) / FishManagerConstant::FISH_ARRAY_SIZE;
    //"lightweight" level implementation: pseudo uniform distribution into an array
    //                                     (remainder makes it bottom heavy)
    //pre-generated should usually be lighter
    //evil bittwiddling candidate 
    //  - O(1) searches
    //alternative: heavier, more rng implementation: hash table
    for (; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++, minY += increment) {
        //generate species
        fish_array_[iFish].species = &fish_species_array_[inclusive_species_distribution_(world.engine())];
        //generate direction
        fish_array_[iFish].velocity = 1 - (world.randomBool() << 1);
        //generate initial x position
        fish_array_[iFish].head_x = world.randomStageX();

        fish_array_[iFish].head_x -= (fish_array_[iFish].velocity > 0) * (fish_array_[iFish].species->sprite_right.maxWidth() - 1);
        fish_array_[iFish].head_y = minY + inclusive_base_fish_y_distribution_(world.engine());
    }
    std::uniform_int_distribution<int> inclusive_last_fish_y_distribution_(minY - increment, WorldConstant::STAGE_LAST_Y);
    fish_array_[FishManagerConstant::FISH_ARRAY_SIZE - 1].head_y = +inclusive_last_fish_y_distribution_(world.engine());
}

void FishManager::update() {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::alive) {
            fish_array_[iFish].update();
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