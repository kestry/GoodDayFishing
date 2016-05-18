//Game.cpp  -- implementation file for Sprite class
//updated 2016/05/16

//Good Day Fishing 
//a game written by Jean Park
//created April 2016

#include <chrono>
#include <iostream>
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
    , state_(GameState::running)
    , poop_message_ticks_(0)
    , fish_message_ticks_(0) {

    for (int i = 0; i < NUM_PLAYER_SPRITES; i++) {
        player_sprites_[i].load(PLAYER_FILENAMES[i]);
    }
    player_ = make_unique<Player>(player_sprites_[0], player_sprites_[1], player_sprites_[2], player_sprites_[3]);
}

//default ~Game(): unique pointers will destroy objects

void Game::init() {
    world_->update();
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
    handleFishToHookCollision();
}

void Game::draw() {
    world_->draw();
    game_object_manager_->draw(*world_);
    player_->draw(*world_);
}

void Game::lateUpdate() {
    game_object_manager_->lateUpdate(*world_);
    handlePoopToHeadCollision();
    if (player_->health() <= 0) {
        state_ = GameState::game_over;
    }
}
void Game::handleFishToHookCollision() {
    --fish_message_ticks_;
    if (game_object_manager_->isFishingCollision()) {
        player_->score() += game_object_manager_->points();
        fish_name_ = game_object_manager_->currentCatch();
        fish_message_ticks_ = GameConstant::FISH_MESSAGE_TICKS;
    }
    if (fish_message_ticks_ <= 0) {
        fish_message_ticks_ = 0;
        fish_name_ = " ";
    }
}

void Game::handlePoopToHeadCollision() {
    if (isPoopToHeadCollision()) {
        player_->damage();
        poop_message_ticks_ = GameConstant::POOP_MESSAGE_TICKS;
    }
}

bool Game::isPoopToHeadCollision() {
    return game_object_manager_->isPoopCollision(player_->headX(), player_->headY());
}

void Game::lateDraw() {
    game_object_manager_->lateDraw(*world_);
    if (poop_message_ticks_ > 0) {
        --poop_message_ticks_;
        player_->drawPoopMessage(*world_);
    }
    world_->swap();
}

void Game::render() {
    world_->render();
}

void Game::centeredPrint(std::string s, int y) {
    int x = (WorldConstant::STAGE_WIDTH - s.size()) >> 1;
    for (int i = 0; i < s.size(); i++) {
        world_->drawTile(s[i], x++, y);
    }
}

void Game::titleScreen() {
    world_->drawBorders();
    world_->clearStage();
    centeredPrint(GameConstant::TITLE_MESSAGE, GameConstant::CENTER_Y - 1);
    centeredPrint(GameConstant::AUTHOR_MESSAGE, GameConstant::CENTER_Y);
    world_->swap();
    render();
}

void Game::gameOverScreen() {
    string input;
    system("cls");
    world_->drawBorders();
    world_->clearStage();
    if (GameState::game_over == state_) {
        centeredPrint(GameConstant::GAME_OVER_MESSAGE, GameConstant::CENTER_Y - 1);
        centeredPrint(GameConstant::CONTINUE_MESSAGE, GameConstant::CENTER_Y);
        world_->swap();
        render();
        while (true) {
            if (GetAsyncKeyState(0x59)) {
                state_ = GameState::running;
                player_->reset();
                break;
            }
            if (GetAsyncKeyState(0x4E)) {
                state_ = GameState::exit;
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
}


void Game::endScreen() {
    string input;
    system("cls");
    world_->drawBorders();
    world_->clearStage();
    centeredPrint(GameConstant::GOODBYE_MESSAGE, GameConstant::CENTER_Y);
    world_->swap();
    render();
}

void Game::run() {
    titleScreen();
    cin.ignore();
    auto currentTime = Clock::now();
    double accumulatedTime = 0.0;
    double gameTime = 0.0;
    do {
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
            message_system_.displayHealthMeter(player_->health(), player_->score(), fish_name_);
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        gameOverScreen();
    } while (GameState::exit != state_);
    endScreen();
    this_thread::sleep_for(chrono::milliseconds(200));
}