//Constants.hpp  -- constants for Good Day Fishing game
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#pragma once

namespace WorldConstant {
    //changes here require specific function rewrite(s)
    const int BORDER_WIDTH = 1;  // changing BORDER_WIDTH above 1 requires an initBorders() rewrite
                                 // DESIGN DILEMMA: game dependent on BORDER_CHAR existance?

    //requirement: value greater than BORDER_WIDTH * 2;
    const int FRAME_WIDTH = 79;   // FRAME_WIDTH = STAGE_WIDTH + 2 * BORDER_WIDTH 
    const int FRAME_HEIGHT = 23;  // FRAME_HEIGHT = STAGE_HEIGHT + 2 * BORDER_WIDTH 

    //do not need to change: constants adjusts itself, dependent on above
    //except: change needed if change to non-uniform border-width
    //          (aka top/bottom/side borders have varying widths)
    const int STAGE_WIDTH = FRAME_WIDTH - 2 * BORDER_WIDTH;
    const int STAGE_HEIGHT = FRAME_HEIGHT - 2 * BORDER_WIDTH;
    const int BUFFER_WIDTH = FRAME_WIDTH + 1;
    const int BUFFER_HEIGHT = FRAME_HEIGHT;
    const int STAGE_FIRST_X = BORDER_WIDTH;
    const int STAGE_LAST_X = STAGE_WIDTH - 1 + STAGE_FIRST_X;
    const int STAGE_FIRST_Y = BORDER_WIDTH;
    const int STAGE_LAST_Y = STAGE_HEIGHT - 1 + STAGE_FIRST_Y;

    //changes here may require large changes
    const int NUM_BUFFERS = 2;  // 2  - default
                                // 1  - have backBuffer() return currentBuffer_
                                // 3+ - adjustments must be made to utilize extra buffers

    //requirement: must be within STAGE ranges
    const int DOCK_WIDTH = 5;
    const int DOCK_X = STAGE_WIDTH - DOCK_WIDTH;
    const int WATER_Y = 7;

    const char BORDER_CHAR = '@';
    const char NULL_CHAR = '\0';
}

namespace SpriteConstant {
    //use these constants to draw assets; assets are stored verbatim
    //note: update example if changing constants
    //example:             sprite_in_file_and_vector = " (#) ";
    //                         world_string_original = "-----"
    //                world_string_after_sprite_draw = "-( )-"
    const char SOLID_WHITE_CHAR = '#';
    const char TRANSPARENT_CHAR = ' ';
}

namespace PlayerConstant {
    const int PLAYER_WIDTH = 9;
    const int PLAYER_HEIGHT = 4;
    const int MIN_FIRST_X = WorldConstant::STAGE_FIRST_X;
    const int MIN_FIRST_Y = WorldConstant::WATER_Y - PLAYER_HEIGHT + 1;
    const int MAX_FIRST_X = WorldConstant::DOCK_X - PLAYER_WIDTH + 5;
    const int MAX_FIRST_Y = MIN_FIRST_Y;
    const int MAX_FISHING_DELAY = 3;
    const int MAX_BOATING_SPEED = 3;
    const int MAX_BOATING_STOP_DELAY = 6;
    const int FULL_METER = 5;
}
namespace HookConstant {
    const char HOOK_CHAR = 'j';
    const int HOOK_WEIGHT = 0;
}

enum class Weight {
    hook,       //j
    starfish,   //*
    flounder,   //Q
    goldfish,   //o<
    snapper,    //<'><
    grouper,    //<"))<

};