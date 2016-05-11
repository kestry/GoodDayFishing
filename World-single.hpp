//World.cpp

//Good Day Fishing written by Jean Park
//Date: 2016/04/28

#pragma once

#include <vector>
#include <string>
#include "Windows.h"

const int FRAME_WIDTH = 79;         // FRAME_WIDTH = 2 * BORDER_WIDTH + STAGE_WIDTH
const int FRAME_HEIGHT = 23;        // FRAME_WIDTH = 2 * BORDER_WIDTH + STAGE_WIDTH
const int BORDER_WIDTH = 1;
const int STAGE_WIDTH = 77;    // STAGE_WIDTH = FRAME_WIDTH - 2 * BORDER_WIDTH
const int STAGE_HEIGHT = 21;   // STAGE_HEIGHT = FRAME_HEIGHT - 2 * BORDER_WIDTH
const int DOCK_WIDTH = 5;
const int DOCK_X = STAGE_WIDTH - DOCK_WIDTH;
const int WATER_Y = 7;

class World {
public:
    World();
    std::string& operator[](int y);
    std::string& at(int);
    int end();

    void update();
    void render() const;
    void ebb() { isEbb_ = !isEbb_; }

private:
    bool isEbb_;
    std::vector<std::string> world_;
    std::vector<std::string> worlds_;

    void drawBorders();
    void drawBackground();
    void drawWater();
    void drawDock();
};

static BOOL gotoxy(const WORD x, const WORD y);
