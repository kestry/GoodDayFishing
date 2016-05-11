//Game.cpp  -- implementation file for Sprite class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include <iostream>
#include <thread>
#include <chrono>
#include "Windows.h"
#include "Game.hpp"
using namespace std;
using std::chrono::milliseconds;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point Time_point;
typedef std::chrono::duration<double, std::chrono::seconds::period> TimeInSeconds;

const double TIME_PER_UPDATE(.5); //time in seconds

Game::Game()
    : is_running_(true)
    , sprites_({ Sprite("boating_left"),
    Sprite("boating_right"),
    Sprite("fishing_left"),
    Sprite("fishing_right") })
    , player_(sprites_[0], sprites_[1],sprites_[2],sprites_[3])
{ }

void Game::init() {
    world_.update();
    player_.update(world_);
    world_.swap();
}

void    Game::process() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        is_running_ = false;
    }
    player_.process();
}

void    Game::update() {
    world_.update();
    player_.update(world_);
    world_.swap();
}

void    Game::render() {
    world_.render();
}

void Game::run() {
    auto currentTime = Clock::now();
    double accumulatedTime = 0.0;
    double gameTime = 0.0;
    while (is_running_) {
        init();
        auto newTime = Clock::now();
        double elapsedTime = TimeInSeconds(newTime - currentTime).count();
        currentTime = newTime;
        accumulatedTime += elapsedTime;
        process();
        while (accumulatedTime >= TIME_PER_UPDATE) {
            update();
            accumulatedTime -= TIME_PER_UPDATE;
            gameTime += TIME_PER_UPDATE;
        }
        render();
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << gameTime << endl;
    }
}