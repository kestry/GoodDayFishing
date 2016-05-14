#include "Hook.hpp"
#include <iostream>
using namespace std;
Hook::Hook()
    : hookSpecies_(new FishSpecies("hook", Sprite(HookConstant::HOOK_CHAR), Sprite(HookConstant::HOOK_CHAR), SwallowSize::hook, 0 ))
    , default_hook_(new Fish(*hookSpecies_))
    , num_caught_(0) {
    default_hook_->capture();
    current_hook = *default_hook_;
}

bool Hook::checkFish(Fish* new_fish) {
    if ((new_fish->state != FishState::alive) || 
        (new_fish->species->swallow_size <= current_hook.species->swallow_size) ||
        (current_hook.head_y != new_fish->head_y) || 
        (current_hook.head_y != new_fish->head_y) ||
        (isInXRange(new_fish))) {
        return false;
    }
    return true;
}

bool Hook::isInXRange(Fish* new_fish) {
    int min_x = (current_hook.x_velocity < 0) ? current_hook.head_x : current_hook.head_x - current_hook.width() - 1;
    int max_x = (current_hook.x_velocity > 0) ? current_hook.head_x : current_hook.head_x + current_hook.width() - 1;
    return (min_x < new_fish->head_x) && (new_fish->head_x < max_x);
}

void Hook::catchFish(Fish* new_fish) {
    new_fish->state = FishState::dead;
    current_hook = *new_fish;
    current_hook.state = FishState::caught;
    current_hook.head_x = home_x;
    ++num_caught_;
}

void Hook::reset() {
    current_hook.state = FishState::dead;
    current_hook = *default_hook_;
    num_caught_ = 0;
}

void Hook::draw(World &world) {
    for (int y = home_y; y < current_hook.head_y; y++) {
        world.drawTile('|', home_x, y);
    }
    current_hook.draw(world);
}