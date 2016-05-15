#include "FishManager.hpp"

FishManager::FishManager(World &world)
    : inclusive_species_distribution_(SpeciesConstant::I_FIRST_FISH_SPECIES,
    SpeciesConstant::NUM_SPECIES - 1) //last =  size - 1
    , inclusive_base_y_distribution_(0, GameObjectConstant::FISH_Y_SPACING - 1) { //last =  size - 1
    fish_species_array_ = new Species[SpeciesConstant::NUM_SPECIES];
    fish_array_ = new Fish[GameObjectConstant::GAME_OBJECT_ARRAY_SIZE];
    hook_ = &fish_array_[GameObjectConstant::I_HOOK];
    loadSpecies();
    generateInitialFish(world);
}

FishManager::~FishManager() {
    delete[] fish_species_array_;
    delete[] fish_array_;
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

void FishManager::update() {
    checkHookPosition();
    for (int iObject = 0; iObject < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iObject++) {
        fish_array_[iObject].update();
    }
}

void FishManager::generateBird(World &world) {

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

//void FishManager::generateBird(int iFish, World &world) {
//    
//}
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
        fish_array_[iFish].head_x = (fish_array_[iFish].isLeftward()) ?
            WorldConstant::STAGE_LAST_X : WorldConstant::STAGE_FIRST_X;
    }
}

void FishManager::generateInitialFish(World &world) {
    restHook();
    for (int iFish = GameObjectConstant::I_FIRST_FISH; 
        iFish < GameObjectConstant::GAME_OBJECT_ARRAY_SIZE; iFish++) {
        generateOneFish(iFish, world);
        if (FishState::alive == fish_array_[iFish].state) {
            fish_array_[iFish].head_x = world.randomStageX();
        }
    }
}

void FishManager::castHook(int home_x, int home_y) {
    numHooked_ = 0;
    home_x_ = home_x;
    home_y_ = home_y;
    hook_->head_x = home_x;
    hook_->head_y = home_y;
    hook_->species = &fish_species_array_[SpeciesConstant::I_HOOK_SPECIES];
    hook_->sink();
}

void FishManager::checkHookPosition() {
    if ( isReeling() && hook_->head_y <= home_y_) {
        restHook();
    }
    else if (hook_->head_y >= WorldConstant::STAGE_LAST_Y) {
        reelHook();
    }
}

//note: used in member function, isCatch, below
int fishHash(int y) {
    return GameObjectConstant::I_FIRST_FISH + 
        (y - GameObjectConstant::STAGE_MIN_FISH_Y) / 
        GameObjectConstant::FISH_Y_SPACING;
}

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

void FishManager::doCatch(Fish* fish) {
    fish->kill();
    hook_->reel();
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
        for (int line_y = home_y_; line_y < hook_->head_y; line_y++) {
            world.drawTile('|', home_x_, line_y);
        }
    }
}