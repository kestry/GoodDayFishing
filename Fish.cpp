#include "Fish.hpp"

Fish::Fish() 
    : head_x(0)
    , head_y(0)
    , is_leftward_(true)
    , x_velocity_(0)
    , y_velocity_(0)
    , state(FishState::dead) {
}

Fish::Fish(const Species &newSpecies) 
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
    killOffscreen();
}

void Fish::killOffscreen() {
    //    @                             @
    //  <>< kill here                   ><> kill here
    // <><@ already dead                @><> already dead
    //    @                             @
    if (x_velocity_ < 0 && head_x < WorldConstant::STAGE_FIRST_X - width() + 1 ||
        x_velocity_ > 0 && head_x > WorldConstant::STAGE_LAST_X + width() - 1) {
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
    (is_leftward_) ?
        species->sprite_left.draw(world, head_x, head_y) :
        species->sprite_right.draw(world, 
        //head_x - (size - 1)   example object: "bbbx"  size = 4, where x == head_x and b = body
        head_x + 1 - species->sprite_right.maxWidth(), head_y);
}


