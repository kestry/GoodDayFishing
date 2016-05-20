//Game.hpp  -- header file for Sprite class
//updated 2016/05/16

//Good Day Fishing game written by Jean Park

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
//#include "GameObjectManager.hpp" 
#include "Player.hpp" 
#include "MessageSystem.hpp"

namespace GameConstant {
    const int POOP_MESSAGE_TICKS = 1;   //set 0 to have no poop messages
    const int FISH_MESSAGE_TICKS = 5;   //set 0 to have no fish messages
    const int CENTER_Y = (WorldConstant::STAGE_HEIGHT >> 1) + WorldConstant::STAGE_FIRST_Y;
    const std::string TITLE_MESSAGE = "Good Day Fishing";
    const std::string AUTHOR_MESSAGE = "by Jean Park";
    const std::string GAME_OVER_MESSAGE = "GAME OVER";
    const std::string CONTINUE_MESSAGE = "continue? (y/n)";
    const std::string GOODBYE_MESSAGE = "Hope you have a Good Day!";
}

enum class GameState {
    game_over,
    exit,
    running,
};

class Game {
public:
    Game();
    Game(const Game &) = delete;
    void operator=(const Game &) = delete;

    void    titleScreen();
    void    run();
    void    gameOverScreen();
    void    endScreen();

private:
    std::unique_ptr<World> world_;
    std::unique_ptr<GameObjectManager> game_object_manager_;
    std::unique_ptr<Sprite[]> player_sprites_;
    std::unique_ptr<Player> player_;
    GameState state_;
    MessageSystem message_system_;
    int poop_message_ticks_;
    int fish_message_ticks_;
    std::string fish_name_;

    //listed in call order
    void    init();
    void    process();
    void    update();
    void    draw();
    void    lateUpdate();
    void    handlePoopToHeadCollision();
    void    handleFishToHookCollision();
    bool    isPoopToHeadCollision();
    void    lateDraw();
    void    centeredPrint(std::string s, int y);

    void    render();
};

#endif