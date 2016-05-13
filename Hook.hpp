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
    int head_x;
    int head_y;

    Hook();
    void size() const;

    void setSize();
    void draw(World &world);

private:
    int isHooked_;
    int hook_width_;
    int hook_size_;
    int score_;
};

#endif