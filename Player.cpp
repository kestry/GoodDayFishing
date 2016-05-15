//Player.cpp  -- implementation file for Player class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

//possible bug source: 
//  player width/height coupled with sprite width/height
//  problem if 1) error loading sprite 
//             2) error with sprite design
//             3) player width/height != sprite width/height
#include "Player.hpp"
using namespace PlayerConstant;

Player::Player(const Sprite &boating_left_sprite,
    const Sprite &boating_right_sprite,
    const Sprite &fishing_left_sprite,
    const Sprite &fishing_right_sprite)
    : mode_(Mode::boating)
    , is_rightward_(false)
    , is_upward_(true)
    //, frames_(0)
    //, fishing_delay_(MAX_FISHING_DELAY)
    , boating_speed_(0)
    , boating_stop_delay_(MAX_BOATING_STOP_DELAY)
    , first_x_(MAX_FIRST_X)
    , first_y_(MAX_FIRST_Y)
    , score_(0)
    , health_(FULL_HEALTH)
    , boating_left_sprite_(boating_left_sprite)
    , boating_right_sprite_(boating_right_sprite)
    , fishing_left_sprite_(fishing_left_sprite)
    , fishing_right_sprite_(fishing_right_sprite)
{ }

void Player::process(FishManager &fish_manager) {
    switch (mode_) {
    case Mode::boating:
        boatingProcess();
        break;
    case Mode::sitting:
        sittingProcess(fish_manager);
        break;
    case Mode::fishing:
        fishingProcess(fish_manager);
        break;
    default:
        break;
    }
}

void Player::boatingProcess() {
    if (GetAsyncKeyState(VK_LEFT)) {
        is_rightward_ = false;
        rowBoat();
    }
    else if (GetAsyncKeyState(VK_RIGHT)) {
        is_rightward_ = true;
        rowBoat();
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        sit();
        --head_y_;
    }
    return;
}

void Player::sittingProcess(FishManager &fish_manager) {
    if (GetAsyncKeyState(VK_UP)) {
        boat();
        return;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        cast(fish_manager);
        return;
    }
}

void Player::fishingProcess(FishManager &fish_manager) {
    if (GetAsyncKeyState(VK_UP)) {
        reel(fish_manager);
        return;
    }
}

void Player::update(FishManager &fish_manager) {
    //++frames_;
    switch (mode_) {
    case Mode::boating:
        boatingUpdate();
        return;
    case Mode::sitting:
        return;
    case Mode::fishing:
        fishingUpdate(fish_manager);
        return;
    }
    return;
}

void Player::boatingUpdate() {
    if (boating_speed_ > 0) {
        if (is_rightward_) {
            first_x_ += boating_speed_;
            first_x_ = first_x_ > MAX_FIRST_X ? MAX_FIRST_X : first_x_;
        }
        else {
            first_x_ -= boating_speed_;
            first_x_ = first_x_ < MIN_FIRST_X ? MIN_FIRST_X : first_x_;
        }
        --boating_speed_;
        if (boating_speed_ == 0 && --boating_stop_delay_) {
            ++boating_speed_;
        }
    }
}

void Player::fishingUpdate(FishManager &fish_manager) {
    if (!fish_manager.isFishing()) {
        sit();
        return;
    }
    if (fish_manager.isFishingCollision()) {
        score_ += fish_manager.points();
    }
    return;
}

void Player::lateUpdate(FishManager &fish_manager) {
    if (fish_manager.isPoopCollision(head_x_, head_y_)) {
        --health_;
    }
}

void Player::draw(World &world) {
    (mode_ == Mode::boating) ? drawBoating(world) : drawFishing(world);
}

void Player::drawBoating(World &world) {
    (is_rightward_) ?
        boating_right_sprite_.draw(world, first_x_, first_y_) :
        boating_left_sprite_.draw(world, first_x_, first_y_);
}

void Player::drawFishing(World &world) {
    (is_rightward_) ?
        fishing_right_sprite_.draw(world, first_x_, first_y_) :
        fishing_left_sprite_.draw(world, first_x_, first_y_);
}

