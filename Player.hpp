//Player.hpp  -- header file for Player class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#pragma once

#include "Constants.hpp"
#include "Hook.hpp"
#include "Sprite.hpp"


class Player {
public:
    Player(const Sprite&, const Sprite&, const Sprite&, const Sprite&);
    void process();
    void update(World &);

    //inline mode functions
    void boat();
    void sit();
    void cast();
    void reel();

    void damage();
    int meter() const;
private:
    enum class Mode {
        boating,
        sitting,
        casting,
        reeling
    };

    Mode mode_;
    bool is_leftward_;
    bool is_upward_;
    int fishing_delay_;
    int boating_speed_;
    int boating_stop_delay_;
    int frames_;
    int head_x_;
    int head_y_;
    int first_x_;
    int first_y_;
    int score_;
    int meter_;
    Hook hook_;
    const Sprite &boating_left_sprite_;
    const Sprite &boating_right_sprite_;
    const Sprite &fishing_left_sprite_;
    const Sprite &fishing_right_sprite_;

    //inline helper functions
    void rowBoat();
    void castLine();
    void fishFaster();
    void fishSlower();
    void reelLine();

    void boatingProcess();
    void sittingProcess();
    void castingProcess();
    void reelingProcess();

    void boatingUpdate();
    void castingUpdate();
    void reelingUpdate();

    void drawBoating(World &world);
    void drawFishing(World &world);
};

// inline mode functions ----------------------------------

inline void Player::boat() {
    mode_ = Mode::boating;
    ++head_y_;
    boating_speed_ = 0;
}

inline void Player::sit() {
    mode_ = Mode::sitting;
}

inline void Player::cast() {
    mode_ = Mode::casting;
    hook_.home_y = first_y_;
    hook_.home_x = first_x_ + ((is_leftward_ - 1) & PlayerConstant::PLAYER_WIDTH);
    hook_.head_x = hook_.home_x;
    hook_.head_y = hook_.home_y;
    castLine();
}

inline void Player::reel() {
    mode_ = Mode::reeling;
    reelLine();
}

// boat helper functions -----------------------------

inline void Player::rowBoat() {
    boating_speed_ = PlayerConstant::MAX_BOATING_SPEED;
    boating_stop_delay_ = PlayerConstant::MAX_BOATING_STOP_DELAY;
}

// cast helper functions -----------------------------

inline void Player::castLine() {
    is_upward_ = false;
    fishing_delay_ = PlayerConstant::MAX_FISHING_DELAY;
}

inline void Player::fishFaster() {
    --fishing_delay_;
}

inline void Player::fishSlower() {
    ++fishing_delay_;
}

// reel helper functions -----------------------------

inline void Player::reelLine() {
    is_upward_ = true;
    fishing_delay_ = PlayerConstant::MAX_FISHING_DELAY;
}