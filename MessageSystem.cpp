#include <iostream>
#include <iomanip>
#include "MessageSystem.hpp"
#include "Player.hpp"
using namespace std;
using namespace MessageSystemConstant;

class player;
class world;

void MessageSystem::displayHealthMeter(int health, int score) {
    cout << "GOOD DAY: [";
    char bar = METER_CHAR;
    for (int i = 0; i < health; i++) {
        cout << bar--;
    }
    for (int i = health; i < PlayerConstant::FULL_HEALTH; i++) {
        cout << ' ';
    }
    cout << ']' << left << setw(WorldConstant::FRAME_WIDTH - GOOD_DAY_METER_DISPLAY_WIDTH - SCORE_DISPLAY_WIDTH) << "\t\t\t";
    cout << "score:" << setw(SCORE_SPACE) << right << score << endl;
}