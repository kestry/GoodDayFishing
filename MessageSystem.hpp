//MessageSystem.hpp  -- header file for MessageSystem class
//written 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#ifndef MESSAGE_SYSTEM
#define MESSAGE_SYSTEM

#include "World.hpp"

namespace MessageSystemConstant {
    const unsigned char METER_CHAR = 178;
    const int GOOD_DAY_METER_DISPLAY_WIDTH = 19;
    const int SCORE_DISPLAY_WIDTH = 31;

    const int MESSAGE_SPACE = WorldConstant::FRAME_WIDTH - GOOD_DAY_METER_DISPLAY_WIDTH - SCORE_DISPLAY_WIDTH;
    const int SCORE_SPACE = 11;
}

class MessageSystem {
public:
    void displayHealthMeter(int health, int score, const std::string &fish_name);
private:
};

#endif