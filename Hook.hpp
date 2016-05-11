#pragma once

#include "World.hpp"

class Hook {
public:
    int home_x;
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