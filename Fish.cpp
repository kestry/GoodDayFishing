#include "Fish.hpp"



Fish::Fish() 
    : velocity(1)
    , state(FishState::dead) {

}

Fish::Fish(const FishSpecies &newSpecies) 
    : velocity(1)
    , state(FishState::alive) 
    , species(&newSpecies) {

}

void Fish::update() {
    head_x += velocity;
}

void Fish::draw(World &world) {
    (velocity < 0) ?
        species->sprite_left.draw(world, head_x, head_y) :
        species->sprite_right.draw(world, 
            head_x - species->sprite_right.maxWidth() - 1, head_y);
}

void Fish::killIfOffscreen() {
    if (velocity < 0 && head_x < WorldConstant::STAGE_FIRST_X - size() ||
        velocity > 0 && head_x > WorldConstant::STAGE_LAST_X + size()) {
        state = FishState::dead;
    }
}
