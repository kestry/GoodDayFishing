//World.hpp - header file for double buffer version of World class

//Good Day Fishing written by Jean Park
//Date: 2016/04/28

#pragma once

#include "Windows.h"

#include "Constants.hpp"

class World {
public:
    World();

    void    update();
    void    render();

    //inline public functions
    void    swap();
    void    drawTile(char, int x, int y);
    void    ebb();

private:
    bool    isEbb_;
    char    buffers_[WorldConstant::NUM_BUFFERS]
                    [WorldConstant::BUFFER_HEIGHT]
                    [WorldConstant::BUFFER_WIDTH];
    int     ebbDelay_;
    int     frames_;
    int  backBuffer_;

    //inline private functions
    int  backBuffer() const;
    int  frontBuffer();

    //initiators
    void    initBorders();

    //render helper functions
    void    clearStage();
    void    drawWater();
    void    drawDock();
};

static BOOL gotoxy(const WORD x, const WORD y);

inline void World::swap() { backBuffer_ = 1 - backBuffer_; }
inline void World::drawTile(char c, int x, int y) { buffers_[backBuffer()][y][x] = c; }
inline void World::ebb() { isEbb_ = !isEbb_; }

inline int World::backBuffer() const { return backBuffer_; }
inline int World::frontBuffer() { return 1 - backBuffer_; }