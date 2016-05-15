//Sprite.hpp  -- header file for Sprite class
//updated 2016/05/07

//Good Day Fishing game written by Jean Park

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <vector>
#include "World.hpp"

namespace SpriteConstant {
    //use these constants to draw assets; assets are stored verbatim
    //note: update example if changing constants
    //example:             sprite_in_file_and_vector = " (+_+#) ";
    //                         world_string_original = "~~~~~~~~"
    //                world_string_after_sprite_draw = "~(+_+ )~"
    const std::string FILE_EXTENSION = ".txt";
    const char SOLID_WHITE_CHAR = '#';
    const char TRANSPARENT_CHAR = ' ';
}

class Sprite {
public:
    Sprite();
    Sprite(char c);
    Sprite(const std::string &fileName);

    //inline functions
    int     size() const;
    int     maxWidth() const;
    int     height() const;
    char    tile(int x, int y) const;

    bool    draw(World &world, int stage_x0, int stage_y0) const; //true if full sprite is drawn
    bool    load(const std::string &fileName);
    void    loadLine(const std::string line);
    void    print() const;
private:
    int max_width_;
    std::vector<std::string> sprite_;
};
inline int Sprite::size() const { return max_width_; }
inline int Sprite::maxWidth() const { return max_width_; }
inline int Sprite::height() const { return sprite_.size(); }
inline char Sprite::tile(int x, int y) const { return sprite_[y][x]; }

#endif