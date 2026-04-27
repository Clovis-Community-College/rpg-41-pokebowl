#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Hero;

class Map {
private:
    std::vector<std::string> grid;
    int width, height;
	void loss(int ox, int oy);
	void au(int ox, int oy, int bx, int by, char body, char visor, char border, char visor_border);
public:
    Map();
    void generate();
    char get_tile(int x, int y) const;
    bool is_passable(int x, int y, const Hero& hero) const;
    int get_width() const { return width; }
    int get_height() const { return height; }
};

#endif
