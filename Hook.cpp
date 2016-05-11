#include "Hook.hpp"

Hook::Hook()
    : hook_width_(1) {
}

void Hook::draw(World &world) {
    for (int y = home_y; y < head_y; y++) {
        world.drawTile('|', head_x, y);
    }
    world.drawTile(HookConstant::HOOK_CHAR, head_x, head_y);
}