#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
private:
    std::vector<std::string> grid;
    int width, height;

public:
    Map();
    void generate();
    char get_tile(int x, int y) const;
    int get_width() const { return width; }
    int get_height() const { return height; }
};

#endif
