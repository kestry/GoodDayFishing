//Player.hpp  -- header file for Player class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "FishManager.hpp"
#include "Sprite.hpp"

class FishManager;

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
    //const int MAX_FISHING_DELAY = 3;
    const int MAX_BOATING_SPEED = 3;
    const int MAX_BOATING_STOP_DELAY = 6;
    const int FULL_HEALTH = 3;
}

class Player {
public:
    Player(const Sprite&, const Sprite&, const Sprite&, const Sprite&);
    void process(FishManager &);
    void update(FishManager &);
    void lateUpdate(FishManager &);
    void draw(World &);

    //inline mode functions
    void boat();
    void sit();
    void cast(FishManager &);
    void reel(FishManager &);

    Mode mode();
    int health() const;
private:
    Mode mode_;
    bool is_rightward_;
    bool is_upward_;
    //int frames_;
    //int fishing_delay_;
    int boating_speed_;
    int boating_stop_delay_;
    int head_x_;
    int head_y_;
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
    void sittingProcess(FishManager &);
    void fishingProcess(FishManager &);

    void boatingUpdate();
    void fishingUpdate(FishManager &);

    void drawBoating(World &world);
    void drawFishing(World &world);
};

// inline mode functions ----------------------------------
inline Mode Player::mode() {
    return mode_;
}

inline void Player::boat() {
    mode_ = Mode::boating;
    ++head_y_;
    boating_speed_ = 0;
}

inline void Player::sit() {
    mode_ = Mode::sitting;
}

inline void Player::cast(FishManager &fish_manager) {
    mode_ = Mode::fishing;
    fish_manager.castHook(
        first_x_ + (is_rightward_ - 1 & PlayerConstant::PLAYER_WIDTH - 1),
        first_y_);
}

inline void Player::reel(FishManager &fish_manager) {
    fish_manager.reelHook();
}

// boating helper functions -----------------------------

inline void Player::rowBoat() {
    boating_speed_ = PlayerConstant::MAX_BOATING_SPEED;
    boating_stop_delay_ = PlayerConstant::MAX_BOATING_STOP_DELAY;
}

inline int Player::health() const {
    return health_;
}

// fishing helper functions -----------------------------

//inline void Player::fishFaster() {
//    --fishing_delay_;
//}
//
//inline void Player::fishSlower() {
//    ++fishing_delay_;
//}


// update helper functions ---------------------------


#endif