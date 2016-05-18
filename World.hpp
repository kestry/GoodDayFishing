//World.hpp - header file for double buffer version of World class

//Good Day Fishing written by Jean Park
//Date: 2016/04/28

#ifndef WORLD_HPP
#define WORLD_HPP

#include <random>
#include "Windows.h"

namespace WorldConstant {
    //changes here require specific function rewrite(s)
    const int BORDER_WIDTH = 1;  // changing BORDER_WIDTH above 1 requires an initBorders() rewrite
    // DESIGN DILEMMA: game dependent on BORDER_CHAR existance?

    //requirement: value greater than BORDER_WIDTH * 2;
    const int FRAME_WIDTH = 79;   // FRAME_WIDTH = STAGE_WIDTH + 2 * BORDER_WIDTH 
    const int FRAME_HEIGHT = 23;  // FRAME_HEIGHT = STAGE_HEIGHT + 2 * BORDER_WIDTH 

    //do not need to change: constants adjusts itself, dependent on above
    //except change needed if: change to non-uniform border-width
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

    const char NULL_CHAR = '\0';
    const char BORDER_CHAR = '@';
}

class World {
public:
    World();

    void    update();
    void    draw();
    void    render();

    void    drawBorders();
    void    clearStage();

    //inline public functions
    void    swap();
    char    tile(int x, int y);
    void    drawTile(char, int x, int y);
    void    ebb();
    size_t  randomStageX();
    size_t  randomBool();
    std::mt19937& engine();

private:
    std::random_device random_seed_;
    std::mt19937 mt_engine_;  //fast, high quality, seedable, reproducible, 
                                 //  not cryptographically secure (STL Lavavej)

    std::uniform_int_distribution<int> inclusive_stage_x_distribution_;
    std::uniform_int_distribution<int> inclusive_bool_distribution_;
    bool    isEbb_;
    char    buffers_[WorldConstant::NUM_BUFFERS]
                    [WorldConstant::BUFFER_HEIGHT]
                    [WorldConstant::BUFFER_WIDTH];
    int     ebbDelay_;
    int     frames_;
    int     backBuffer_;

    //inline private functions
    int     backBuffer() const;
    int     frontBuffer();

    //render helper functions
    void    drawWater();
    void    drawDock();
};

static BOOL gotoxy(const WORD x, const WORD y);

inline void World::swap() { backBuffer_ = 1 - backBuffer_; }
inline char World::tile(int x, int y) { return buffers_[backBuffer()][y][x]; }
inline void World::drawTile(char c, int x, int y) { buffers_[backBuffer()][y][x] = c; }
inline void World::ebb() { isEbb_ = !isEbb_; }

inline int World::backBuffer() const { return backBuffer_; }
inline int World::frontBuffer() { return 1 - backBuffer_; }
inline size_t World::randomStageX() { return inclusive_stage_x_distribution_(mt_engine_); }
inline size_t World::randomBool() { return inclusive_bool_distribution_(mt_engine_); }
inline std::mt19937& World::engine() { return mt_engine_; };

#endif