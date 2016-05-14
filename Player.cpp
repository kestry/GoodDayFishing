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
#include <iostream>
using namespace std;
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
    , meter_(FULL_METER)
    , boating_left_sprite_(boating_left_sprite)
    , boating_right_sprite_(boating_right_sprite)
    , fishing_left_sprite_(fishing_left_sprite)
    , fishing_right_sprite_(fishing_right_sprite)
{ }

void Player::process() {
    switch (mode_) {
    case Mode::boating:
        boatingProcess();
        break;
    case Mode::sitting:
        sittingProcess();
        break;
    case Mode::casting:
        castingProcess();
        break;
    case Mode::reeling:
        reelingProcess();
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

void Player::sittingProcess() {
    if (GetAsyncKeyState(VK_UP)) {
        boat();
        return;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        cast();
        return;
    }
}

void Player::castingProcess() {
    if (GetAsyncKeyState(VK_UP)) {
        reel();
        return;
    }
    //if (fishing_delay_ > 0 && GetAsyncKeyState(VK_DOWN)) {
    //    fishFaster();
    //    return;
    //}
}

void Player::reelingProcess() {
    if (hook.current_hook.head_y == hook.home_y) {
        sit();
        return;
    }
    //if (fishing_delay_ > 0 && GetAsyncKeyState(VK_UP)) {
    //    fishFaster();
    //    return;
    //}
    //if (fishing_delay_ < MAX_FISHING_DELAY && GetAsyncKeyState(VK_DOWN)) {
    //    fishSlower();
    //    return;
    //}
}

void Player::update() {
    //++frames_;
    switch (mode_) {
    case Mode::boating:
        boatingUpdate();
        return;
    case Mode::sitting:
        return;
    case Mode::casting:
        castingUpdate();
        return;
    case Mode::reeling:
        reelingUpdate();
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

//cast state --> reel state if:
//1. hit sea floor
//2. catch fish
//    fish becomes hook
//    Process:
//    - fish check for hook
//      - if (fish.head_y == hook.head_y && fish.head_x in hook width range) {
//      -   hook.setHasCaught(true);
//      -   hook.setSprite(Sprite*);
//        }
//      - hook.draw();
//    - hook check for fish
//      - findFishHeadX(hook.head_y)
//      - if (fish.head_y == hook.head_y && fish.head_x in hook width range) {
//      -   hook.setHasCaught(true);
//      -   hook.setSprite(Sprite*);
//        }
//      - hook.draw();
void Player::castingUpdate() {
    cerr << "Player::castingUpdate(World &world)" << endl;

    //if (frames_ >= fishing_delay_) {
    //    frames_ = 0;
        ++hook.current_hook.head_y;
    //}
    if (hook.current_hook.head_y > WorldConstant::STAGE_LAST_Y) {
        hook.current_hook.head_y = WorldConstant::STAGE_LAST_Y;
        reel();
    }
    return;
}

//bigger fish becomes hook
void Player::reelingUpdate() {
    //if (frames_ >= fishing_delay_) {
    //    frames_ = 0;
        --hook.current_hook.head_y;
    //}
    if (hook.current_hook.head_y <= hook.home_y) {
        mode_ = Mode::sitting;
        hook.reset();
        //updateScore();
        //resetHook();
    }
    return;
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

