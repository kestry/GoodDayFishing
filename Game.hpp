//Game.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "FishManager.hpp" 
#include "Player.hpp" 

enum class GameState {
    game_over,
    exit,
    running,
};
class Game {
public:
    Game();
    void run();
    const GameState& state();

private:
    std::unique_ptr<World> world_;
    std::unique_ptr<FishManager> fish_manager_;
    std::unique_ptr<Sprite[]> player_sprites_;
    std::unique_ptr<Player> player_;
    GameState state_;

    void    init();
    void    process();
    void    update();
    void    draw();
    void    lateUpdate();
    void    lateDraw();

    void    render();
};

#endif