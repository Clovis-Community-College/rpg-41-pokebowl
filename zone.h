#ifndef ZONE_H
#define ZONE_H

#include <vector>
#include <string>
#include "actor.h"

struct ZoneTile {
    char ch;
    int color_pair;
};

class Zone {
private:
    std::vector<std::vector<ZoneTile>> grid;
    int width, height;
    XY spawn;
    XY exit_pt;

public:
    Zone();
    void load_inn();
    ZoneTile get_tile(int x, int y) const;
    bool is_passable(int x, int y) const;
    bool is_exit(int x, int y) const;
    bool is_npc(int x, int y) const;
    int get_width() const;
    int get_height() const;
    XY get_spawn() const;
};

#endif
