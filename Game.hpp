//Game.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

/*
Personal Notes:

Suprisingly difficult decisions that MAY not mean anything.
For example:
Player in World?
or Player in Game?


*/

#pragma once

#include "Player.hpp" 
    //includes Constants.hpp, Hook.hpp, Sprite.hpp, World.hpp
        //Sprite.hpp includes <vector>, <string>
 

class Game {
public:
    Game();
    void    run();

private:
    bool    is_running_;
    std::vector<Sprite> sprites_;
    World   world_;
    Player  player_;

    void    init();
    void    process();
    void    update();
    void    render();
};