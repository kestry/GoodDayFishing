#include "Fish.hpp"

Fish::Fish() 
    : head_x(0)
    , head_y(0)
    , x_velocity(0)
    , y_velocity(0)
    , state(FishState::dead) {
}

Fish::Fish(const FishSpecies &newSpecies) 
    : head_x(0)
    , head_y(0)
    , x_velocity(1)
    , y_velocity(0)
    , state(FishState::alive) 
    , species(&newSpecies) {
}

void Fish::update() {
    head_x += x_velocity;
    head_y += y_velocity;
}

void Fish::draw(World &world) {
    (x_velocity <= 0) ?
        species->sprite_left.draw(world, head_x, head_y) :
        species->sprite_right.draw(world, 
            head_x - species->sprite_right.maxWidth() - 1, head_y);
}

void Fish::killOffscreenFish() {
    if (x_velocity < 0 && head_x < WorldConstant::STAGE_FIRST_X - width() ||
        x_velocity > 0 && head_x > WorldConstant::STAGE_LAST_X + width()) {
        kill();
    }
}

void Fish::revive() {
    state = FishState::alive;
    x_velocity = 1;
    y_velocity = 0;
}

void Fish::capture() {
    state = FishState::caught;
    x_velocity = 0;
    y_velocity = -1;
}

void Fish::kill() {
    state = FishState::dead;
    x_velocity = 0;
    y_velocity = 0;
}