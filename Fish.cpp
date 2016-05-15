#include "Fish.hpp"

Fish::Fish() 
    : head_x(0)
    , head_y(0)
    , is_leftward_(false)
    , x_velocity_(0)
    , y_velocity_(0)
    , state(FishState::dead) {
}

Fish::Fish(const FishSpecies &newSpecies) 
    : head_x(0)
    , head_y(0)
    , is_leftward_(false)
    , x_velocity_(1)
    , y_velocity_(0)
    , state(FishState::alive) 
    , species(&newSpecies) {
}

void Fish::update() {
    head_x += x_velocity_;
    head_y += y_velocity_;
    killOffscreenFish();
}

void Fish::killOffscreenFish() {
    if (x_velocity_ < 0 && head_x < WorldConstant::STAGE_FIRST_X - width() ||
        x_velocity_ > 0 && head_x > WorldConstant::STAGE_LAST_X + width()) {
        kill();
    }
}

void Fish::revive() {
    state = FishState::alive;
    is_leftward_ = false;
    x_velocity_ = 1;
    y_velocity_ = 0;
}

void Fish::reverse() {
    is_leftward_ = !is_leftward_;
    x_velocity_ = -x_velocity_;
}

void Fish::sink() {
    state = FishState::caught;
    x_velocity_ = 0;
    y_velocity_ = 1;
}

void Fish::reel() {
    state = FishState::caught;
    x_velocity_ = 0;
    y_velocity_ = -1;
}

void Fish::kill() {
    state = FishState::dead;
    x_velocity_ = 0;
    y_velocity_ = 0;
}

void Fish::draw(World &world) {
    (x_velocity_ <= 0) ?
        species->sprite_left.draw(world, head_x, head_y) :
        species->sprite_right.draw(world, 
            head_x - species->sprite_right.maxWidth() - 1, head_y);
}


