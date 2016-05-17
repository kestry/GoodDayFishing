//Game.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "GameObjectManager.hpp" 
#include "Player.hpp" 
#include "MessageSystem.hpp"

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

    void run();
    const GameState& state();

private:
    std::unique_ptr<World> world_;
    std::unique_ptr<GameObjectManager> game_object_manager_;
    std::unique_ptr<Sprite[]> player_sprites_;
    std::unique_ptr<Player> player_;
    GameState state_;
    MessageSystem message_system_;

    //listed in call order
    void    init();
    void    process();
    void    update();
    void    draw();
    void    lateUpdate();
    bool    isPoopHeadCollision();
    void    lateDraw();

    void    render();
};

#endif