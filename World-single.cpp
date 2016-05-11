//World.cpp

//Good Day Fishing written by Jean Park
//Date: 2016/04/28

#include <iostream>
#include "World-single.hpp"
using namespace std;

World::World() : isEbb_(false) {
    //worlds_.at(0).reserve()
    world_.reserve(30);
    drawBorders();
}

void World::update() {
    drawBackground();
    drawWater();
    drawDock();
}

string& World::operator[](int y) {
    return world_[y];
}

string& World::at(int y) {
    return world_.at(y);
}

void World::drawBorders() {
    string line_border(FRAME_WIDTH, '@');
    string side_border(FRAME_WIDTH, ' ');
    side_border.front() = side_border.back() = '@';
    world_.push_back(line_border);
    for (int i = 0; i < STAGE_HEIGHT; i++) {
        world_.push_back(side_border);
    }
    world_.push_back(line_border);
}

void World::drawBackground() {
    for (int y = BORDER_WIDTH; y < STAGE_HEIGHT; y++) {
        for (int x = BORDER_WIDTH; x < STAGE_WIDTH; x++) {
            world_[y][x] = ' ';
        }
    }
}

void World::drawWater() {
    for (int i = BORDER_WIDTH + !isEbb_; i < DOCK_X; i += 2) {
        world_[WATER_Y][i] = '~';
    }
    for (int i = BORDER_WIDTH + isEbb_; i < DOCK_X; i += 2) {
        world_[WATER_Y][i] = '-';
    }
}

void World::drawDock() { 
    //draw surface
    for (int i = DOCK_X; i < STAGE_WIDTH + BORDER_WIDTH; i++) {
        world_[WATER_Y - 1][i] = '_';
    }
    //draw pillars
    for (int i = DOCK_X; i < STAGE_WIDTH + BORDER_WIDTH; i++) {
        world_[WATER_Y][i] = '|';
    }
    //erase pillar
    for (int i = DOCK_X + 2; i < STAGE_WIDTH + BORDER_WIDTH; i += 3) {
        world_[WATER_Y][i] = ' ';
    }
}

void World::render() const {
    gotoxy(0, 0);
    for (const string& line : world_) {
        cout << line << endl;
    }
    return;
}

BOOL gotoxy(const WORD x, const WORD y) {
    COORD xy;
    xy.X = x;
    xy.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}
