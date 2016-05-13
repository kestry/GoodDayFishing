//Game.cpp  -- implementation file for Sprite class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include <chrono>
#include <iostream>
#include <thread>
#include "Windows.h"
#include "Game.hpp"
using namespace std;
using std::chrono::milliseconds;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point Time_point;
typedef std::chrono::duration<double, std::chrono::seconds::period> TimeInSeconds;

const double TIME_PER_UPDATE(.5); //time in seconds

const int NUM_PLAYER_SPRITES = 4;
const std::string PLAYER_FILENAMES[] = { "assets/boating_left.txt"
    , "assets/boating_right.txt"
    , "assets/fishing_left.txt"
    , "assets/fishing_right.txt" };

Game::Game()
    : world_(new World)
    , player_sprites_(new Sprite[NUM_PLAYER_SPRITES])
    , fish_manager_(new FishManager(*world_))
    , is_running_(true) {
    for (int i = 0; i < NUM_PLAYER_SPRITES; i++) {
       player_sprites_[i].load(PLAYER_FILENAMES[i]);
    }
    player_ = make_unique<Player>(player_sprites_[0], player_sprites_[1], player_sprites_[2], player_sprites_[3]);
}

void Game::init() {
    world_->update();
    player_->update(*world_);
    //fish_manager_->update();
    fish_manager_->draw(*world_);
    world_->swap();
}

void    Game::process() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        is_running_ = false;
    }
    player_->process();
}

void    Game::update() {
    world_->update();
    player_->update(*world_);
    //fish_manager_->update();
    fish_manager_->draw(*world_);
    world_->swap();
}

void    Game::render() {
    world_->render();
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