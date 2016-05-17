#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Species.hpp"
#include "World.hpp"

enum class GameObjectState {
    pending,    //transition state (example: poop pending death or draw)
    dead,       //stationary
    caught,     //moves vertically
    alive       //moves horizontally
};

class GameObject {
public:
    int head_x;
    int head_y;
    GameObjectState state;
    const Species* species;

    GameObject();
    GameObject(const Species &species);
    
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

inline int GameObject::width() const { return species->sprite_left.size(); }
inline bool GameObject::isLeftward() const { return is_leftward_; }
inline bool GameObject::isUpward() const { return -1 == y_velocity_; }
inline void GameObject::animate() { is_leftward_ = !is_leftward_; }

#endif