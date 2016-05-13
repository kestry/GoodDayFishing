//Game.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include "Player.hpp" 
#include "FishManager.hpp" 

class Game {
public:
    Game();
    void    run();

private:
    std::unique_ptr<World> world_;
    std::unique_ptr<Sprite[]> player_sprites_;
    std::unique_ptr<FishManager> fish_manager_;
    std::unique_ptr<Player> player_;
    bool                is_running_;

    void    init();
    void    process();
    void    update();
    void    render();
};

#endif