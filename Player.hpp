//Player.hpp  -- header file for Player class
//updated 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObjectManager.hpp"
#include "Sprite.hpp"

class GameObjectManager;

enum class Mode {
    boating,
    sitting,
    fishing
};

namespace PlayerConstant {
    const int PLAYER_WIDTH = 9;
    const int PLAYER_HEIGHT = 4;
    const int MIN_FIRST_X = WorldConstant::STAGE_FIRST_X;
    const int MIN_FIRST_Y = WorldConstant::WATER_Y - PLAYER_HEIGHT + 1;
    const int MAX_FIRST_X = WorldConstant::DOCK_X - PLAYER_WIDTH + 5;
    const int MAX_FIRST_Y = MIN_FIRST_Y;
    const int MAX_BOATING_SPEED = 3;
    const int MAX_BOATING_STOP_DELAY = 6;
    const int FULL_HEALTH = 3;
    const Sprite POOP_MESSAGE("ew");
}

class Player {
public:
    Player(const Sprite&, const Sprite&, const Sprite&, const Sprite&);
    void process(GameObjectManager &);
    void update(GameObjectManager &);
    void lateUpdate(GameObjectManager &);
    void draw(World &);
    void reset();

    //inline mode functions
    void boat();
    void sit();
    void cast(GameObjectManager &);
    void reel(GameObjectManager &);

    Mode mode() const;
    int health() const;
    int& score();
    int headX() const;
    int headY() const;
    void damage();
    void drawPoopMessage(World &);

private:
    Mode mode_;
    bool is_rightward_;
    bool is_upward_;
    int boating_speed_;
    int boating_stop_delay_;
    int first_x_;
    int first_y_;
    int score_;
    int health_;
    const Sprite &boating_left_sprite_;
    const Sprite &boating_right_sprite_;
    const Sprite &fishing_left_sprite_;
    const Sprite &fishing_right_sprite_;

    //inline helper functions
    void rowBoat();
    //void fishFaster();
    //void fishSlower();

    void boatingProcess();
    void sittingProcess(GameObjectManager &);
    void fishingProcess(GameObjectManager &);

    void boatingUpdate();
    void fishingUpdate(GameObjectManager &);

    void drawBoating(World &);
    void drawFishing(World &);
};

// inline mode functions ----------------------------------
inline Mode Player::mode() const {
    return mode_;
}

inline void Player::boat() {
    mode_ = Mode::boating;
    boating_speed_ = 0;
}

inline void Player::sit() {
    mode_ = Mode::sitting;
}

inline void Player::cast(GameObjectManager &GameObject_manager) {
    mode_ = Mode::fishing;
    GameObject_manager.castHook(
        first_x_ + (is_rightward_ - 1 & PlayerConstant::PLAYER_WIDTH - 1),
        first_y_);
}

inline void Player::reel(GameObjectManager &GameObject_manager) {
    GameObject_manager.reelHook();
}

// boating helper functions -----------------------------

inline void Player::rowBoat() {
    boating_speed_ = PlayerConstant::MAX_BOATING_SPEED;
    boating_stop_delay_ = PlayerConstant::MAX_BOATING_STOP_DELAY;
}

inline int Player::health() const {
    return health_;
}

inline int& Player::score() {
    return score_;
}

inline int Player::headX() const {
    return first_x_ + 3 + is_rightward_;
}

inline int Player::headY() const {
    return first_y_ + (mode_ != Mode::boating);
}

inline void Player::damage(){
    --health_;
}



#endif