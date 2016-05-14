#include "FishManager.hpp"

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
    loadOneSpecies(1, "starfish", "*", "*", SwallowSize::starfish, 500);
    loadOneSpecies(2, "jellyfish", "Q", "Q", SwallowSize::starfish, -800);
    loadOneSpecies(3, "goldfish", "o<", ">o", SwallowSize::goldfish, 300);
    loadOneSpecies(4, "mackerel", "<><", "><>", SwallowSize::mackerel, 700);
    loadOneSpecies(5, "tuna", "<\")<", ">(\">", SwallowSize::tuna, 1000);
}

void FishManager::loadOneSpecies(int index, std::string name, std::string ascii_left, std::string ascii_right, SwallowSize swallow_size, int points) {
    fish_species_array_[index].name = name;
    fish_species_array_[index].sprite_left = ascii_left;
    fish_species_array_[index].sprite_right = ascii_right;
    fish_species_array_[index].swallow_size = swallow_size;
    fish_species_array_[index].points = points;
}

void FishManager::update() {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        fish_array_[iFish].update();
        fish_array_[iFish].killOffscreenFish();
    }
}

void FishManager::generateFish(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::dead) {
            generateOneFish(iFish, world);
        }
    }
}

//Prerequisite: iFish fish must be dead
//  reason: otherwise will change the caught or live fish
//  note: can generate over "caught" fish if hook copies fish traits
void FishManager::generateOneFish(int iFish, World &world) {
    //generate live or dead
    if (world.randomBool()) {
        fish_array_[iFish].revive();
        if (world.randomBool()) {
            fish_array_[iFish].swapDirection();
        }
        fish_array_[iFish].head_y = FishManagerConstant::STAGE_MIN_FISH_Y + randomBaseY(world) + iFish * FishManagerConstant::FISH_Y_SPACING;
        fish_array_[iFish].species = &randomSpecies(world);
        fish_array_[iFish].head_x = (fish_array_[iFish].x_velocity < 0) ?
            WorldConstant::STAGE_LAST_X : WorldConstant::STAGE_FIRST_X;
    }
}

void FishManager::generateInitialFish(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        generateOneFish(iFish, world);
        if (FishState::alive == fish_array_[iFish].state) {
            fish_array_[iFish].head_x = world.randomStageX();
        }
    }
}

int fishHash(int y) {
    return (y - FishManagerConstant::STAGE_MIN_FISH_Y)
        / FishManagerConstant::FISH_Y_SPACING;
}

//void FishManager::updateHook(Player &player) {
//    ////if hook is in fish zone
//    if (player.mode() > Mode::sitting && player.hook.current_hook->head_y >= FishManagerConstant::STAGE_MIN_FISH_Y) {
//        //get corresponding fish
//        int iFish = fishHash(player.hook.current_hook->head_y);
//        //check if corresponding fish is caught
//        cout << fish_array_[iFish].species->name << endl;
//        if (fish_array_[iFish].state == FishState::alive && player.hook.checkFish(&fish_array_[iFish])) {
//            player.reel();
//            fish_array_[iFish].state = FishState::caught;
//            player.hook.catchFish(&fish_array_[iFish]);
//            player.addPoints(player.hook.points());
//        }
//    }
//}


void FishManager::draw(World &world) {
    for (int iFish = 0; iFish < FishManagerConstant::FISH_ARRAY_SIZE; iFish++) {
        if (fish_array_[iFish].state == FishState::alive) {
            fish_array_[iFish].draw(world);
        }
    }
}