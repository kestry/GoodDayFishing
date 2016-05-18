//MessageSystem.cpp  -- implementation file for MessageSystem  class
//written 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include <iostream>
#include <iomanip>
#include "MessageSystem.hpp"
#include "Player.hpp"
using namespace std;
using namespace MessageSystemConstant;

void MessageSystem::displayHealthMeter(int health, int score, const std::string &fish_name) {
    cout << "GOOD DAY: [";
    char bar = METER_CHAR;
    for (int i = 0; i < health; i++) {
        cout << bar--;
    }
    for (int i = health; i < PlayerConstant::FULL_HEALTH; i++) {
        cout << ' ';
    }
    cout << "]\t\t\t" << left << setw(WorldConstant::FRAME_WIDTH - GOOD_DAY_METER_DISPLAY_WIDTH - SCORE_DISPLAY_WIDTH) << fish_name;
    cout << "score:" << setw(SCORE_SPACE) << right << score << endl;
}