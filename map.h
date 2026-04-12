#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Hero;

class Map {
private:
    std::vector<std::string> grid;
    int width, height;

public:
    Map();
    void generate();
    char get_tile(int x, int y) const;
    bool is_passable(int x, int y, const Hero& hero) const;
    int get_width() const { return width; }
    int get_height() const { return height; }
};

#endif
