//GameObject.cpp  -- implementation file for GameObject class
//updated 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include "GameObject.hpp"

GameObject::GameObject() 
    : head_x(0)
    , head_y(0)
    , is_leftward_(true)
    , x_velocity_(0)
    , y_velocity_(0)
    , state(GameObjectState::dead) {
}

GameObject::GameObject(const Species &newSpecies) 
    : head_x(0)
    , head_y(0)
    , is_leftward_(false)
    , x_velocity_(1)
    , y_velocity_(0)
    , state(GameObjectState::alive) 
    , species(&newSpecies) {
}

void GameObject::update() {
    head_x += x_velocity_;
    head_y += y_velocity_;
    killOffscreen();
}

void GameObject::killOffscreen() {
    //    @                             @
    //  <>< kill here                   ><> kill here
    // <><@ already dead                @><> already dead
    //    @                             @
    if (x_velocity_ < 0 && head_x < WorldConstant::STAGE_FIRST_X - width() + 1 ||
        x_velocity_ > 0 && head_x > WorldConstant::STAGE_LAST_X + width() - 1) {
        kill();
    }
}

void GameObject::revive() {
    state = GameObjectState::alive;
    is_leftward_ = false;
    x_velocity_ = 1;
    y_velocity_ = 0;
}

void GameObject::reverse() {
    is_leftward_ = !is_leftward_;
    x_velocity_ = -x_velocity_;
}

void GameObject::sink() {
    state = GameObjectState::caught;
    x_velocity_ = 0;
    y_velocity_ = 1;
}

void GameObject::reel() {
    state = GameObjectState::caught;
    x_velocity_ = 0;
    y_velocity_ = -1;
}

void GameObject::kill() {
    state = GameObjectState::dead;
    x_velocity_ = 0;
    y_velocity_ = 0;
}

void GameObject::draw(World &world) {
    (is_leftward_) ?
        species->sprite_left.draw(world, head_x, head_y) :
        species->sprite_right.draw(world, 
        //head_x - (size - 1)   example object: "bbbx"  size = 4, where x == head_x and b = body
        head_x + 1 - species->sprite_right.maxWidth(), head_y);
}