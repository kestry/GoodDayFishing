//Game.cpp  -- implementation file for Sprite class
//updated 2016/05/09

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include <chrono>

#include <thread>
#include "Windows.h"
#include "Game.hpp"
#include "MessageSystem.hpp"
using namespace std;
using std::chrono::milliseconds;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point Time_point;
typedef std::chrono::duration<double, std::chrono::seconds::period> TimeInSeconds;

const double TIME_PER_UPDATE(.25); //time in seconds

const int NUM_PLAYER_SPRITES = 4;
const std::string PLAYER_FILENAMES[] = { "assets/boating_left.txt"
, "assets/boating_right.txt"
, "assets/fishing_left.txt"
, "assets/fishing_right.txt" };

Game::Game()
    : world_(new World)
    , player_sprites_(new Sprite[NUM_PLAYER_SPRITES])
    , game_object_manager_(new GameObjectManager(*world_))
    , state_(GameState::running) {
    for (int i = 0; i < NUM_PLAYER_SPRITES; i++) {
        player_sprites_[i].load(PLAYER_FILENAMES[i]);
    }
    player_ = make_unique<Player>(player_sprites_[0], player_sprites_[1], player_sprites_[2], player_sprites_[3]);
}

//default ~Game(): unique pointers will destroy objects

void Game::init() {
    world_->update();
    //needed for first hook somehow...
    game_object_manager_->regenerate(*world_);
    draw();
    world_->swap();
}

void Game::process() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        state_ = GameState::exit;
    }
    player_->process(*game_object_manager_);
}

void Game::update() {
    //regenerate world
    game_object_manager_->regenerate(*world_);

    //update world
    world_->update();
    game_object_manager_->update();
    player_->update(*game_object_manager_);
}

void Game::draw() {
    world_->draw();
    game_object_manager_->draw(*world_);
    player_->draw(*world_);
}

void Game::lateUpdate() {
    game_object_manager_->lateUpdate(*world_);
    player_->lateUpdate(*game_object_manager_);
    if (isPoopHeadCollision()) {
        player_->damage();
    }
    if (player_->health() <= 0) {
        state_ = GameState::game_over;
    }
}

bool Game::isPoopHeadCollision() {
    return game_object_manager_->isPoopCollision(player_->headX(), player_->headY());
}

void Game::lateDraw() {
    game_object_manager_->lateDraw(*world_);
    world_->swap();
}

void Game::render() {
    world_->render();
    game_object_manager_->draw(*world_);
}

void Game::run() {
    auto currentTime = Clock::now();
    double accumulatedTime = 0.0;
    double gameTime = 0.0;
    while (state_ == GameState::running) {
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
            draw();
            lateUpdate();
            lateDraw();
        }
        render();
        message_system_.displayHealthMeter(player_->health(), player_->score());
        this_thread::sleep_for(chrono::milliseconds(200));
    }

}