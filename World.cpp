//World.cpp - double buffer version of World class
//updated 2016/05/06

//Good Day Fishing written by Jean Park

#include <iostream>
#include "World.hpp"
using std::cout;
using std::endl;
using namespace WorldConstant;

// Constructors ===========================================

World::World() 
    : mt_engine_(random_seed_())
    , inclusive_stage_x_distribution_(STAGE_FIRST_X, STAGE_LAST_X)
    , inclusive_bool_distribution_(0, 1) 
    , isEbb_(false)
    , ebbDelay_(6)
    , frames_(0)
    , backBuffer_(0) {
    initBorders();
}

// Initiators =============================================

void World::initBorders() {
    //initializes frame for all buffers
    for (int i = 0; i < NUM_BUFFERS; i++) {
        //draw top and bottom borders
        for (int k = 0; k < FRAME_WIDTH; k++) {
            buffers_[i][0][k] = BORDER_CHAR;
            buffers_[i][FRAME_HEIGHT - 1][k] = BORDER_CHAR;
        }
        //null-terminate top and bottom borders
        buffers_[i][0][FRAME_WIDTH] = NULL_CHAR;
        buffers_[i][FRAME_HEIGHT - 1][FRAME_WIDTH] = NULL_CHAR;
        //draw side borders and null-terminator
        for (int j = 1; j < FRAME_HEIGHT - BORDER_WIDTH; j++) {
            buffers_[i][j][0] = BORDER_CHAR;
            buffers_[i][j][FRAME_WIDTH - 1] = BORDER_CHAR;
            buffers_[i][j][FRAME_WIDTH] = NULL_CHAR;
        }
    }
}

// Update =================================================

void World::update() {
    ++frames_;
    if (frames_ >= ebbDelay_) {
        ebb();
        frames_ = 0;
    }
    clearStage();
    drawWater();
    drawDock();
}

void World::clearStage() {
    for (int y = STAGE_FIRST_X; y <= STAGE_LAST_Y; y++) {
        for (int x = STAGE_FIRST_X; x <= STAGE_LAST_X; x++) {
            buffers_[backBuffer()][y][x] = ' ';
        }
    }
}

void World::drawWater() {
    for (int i = STAGE_FIRST_X + !isEbb_; i < DOCK_X; i += 2) {
        buffers_[backBuffer()][WATER_Y][i] = '~';
    }
    for (int i = STAGE_FIRST_X + isEbb_; i < DOCK_X; i += 2) {
        buffers_[backBuffer()][WATER_Y][i] = '-';
    }
}

void World::drawDock() {
    //draw surface
    for (int i = DOCK_X; i <= STAGE_LAST_X; i++) {
        buffers_[backBuffer()][WATER_Y - 1][i] = '_';
    }
    //draw pillars
    for (int i = DOCK_X; i <= STAGE_LAST_X; i++) {
        buffers_[backBuffer()][WATER_Y][i] = '|';
    }
    //erase pillar
    for (int i = DOCK_X + 2; i <= STAGE_LAST_X; i += 3) {
        buffers_[backBuffer()][WATER_Y][i] = ' ';
    }
}

// Render =================================================

void World::render() {
    gotoxy(0, 0);
    int  front = frontBuffer();
    for (int y = 0; y < FRAME_HEIGHT; y++) {
        cout << buffers_[front][y] << endl;
    }
}

BOOL gotoxy(const WORD x, const WORD y) {
    COORD xy;
    xy.X = x;
    xy.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}