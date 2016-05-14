#ifndef HOOK_HPP
#define HOOK_HPP

#include "Fish.hpp"
#include "World.hpp"

namespace HookConstant {
    const char HOOK_CHAR = 'j';
}

class Hook {
public:
    int home_y;
    int home_x;
    Fish current_hook;

    Hook();

    int points();
  
    bool checkFish(Fish* newFish);
    void catchFish(Fish* newFish);
    void Hook::reset();
    void draw(World &world);
private:
    const FishSpecies* const hookSpecies_;
    Fish* default_hook_;
    int num_caught_;

    int multiplier();
    bool isInXRange(Fish* newFish);
};

inline int Hook::points() { return multiplier() * current_hook.species->points; }
inline int Hook::multiplier() { return num_caught_ + 1; }

#endif