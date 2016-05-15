#ifndef FISH_HPP
#define FISH_HPP

#include "Species.hpp"
#include "World.hpp"

enum class FishState {
    pending,    //transition state (example: poop pending death or draw)
    dead,       //stationary
    caught,     //moves vertically
    alive       //moves horizontally
};

class Fish {
public:
    int head_x;
    int head_y;
    FishState state;
    const Species* species;

    Fish();
    Fish(const Species &species);
    
    void update();
    void draw(World &world);

    void revive();
    void reverse();
    void sink();
    void reel();
    void kill();
    void animate();

    int width() const;
    bool isLeftward() const;
    bool isUpward() const;
private:
    bool is_leftward_;
    int x_velocity_;
    int y_velocity_;

    void killOffscreen();
};

inline int Fish::width() const { return species->sprite_left.size(); }
inline bool Fish::isLeftward() const { return is_leftward_; }
inline bool Fish::isUpward() const { return -1 == y_velocity_; }
inline void Fish::animate() { is_leftward_ = !is_leftward_; }

#endif