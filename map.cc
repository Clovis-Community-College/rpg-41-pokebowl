#include "map.h"
#include "actor.h"

Map::Map() : width(200), height(200) {
  grid.resize(height, std::string(width, '.'));
}

void Map::generate() {
  for (int y = 10; y < 30; ++y) {
    for (int x = 20; x < 40; ++x) {
      grid[y][x] = '~';
    }
  }

  for (int y = 90; y < 110; ++y) {
    for (int x = 90; x < 110; ++x) {
      if (y == 90 || y == 109 || x == 90 || x == 109) {
        if (x != 100)
          grid[y][x] = '#';
      } else {
        grid[y][x] = ' ';
      }
    }
  }

  grid[95][95] = 'H';
  grid[95][96] = 'H';
  grid[105][95] = 'H';
  grid[95][105] = 'I';
  grid[105][105] = 'S';
}

char Map::get_tile(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return '~';
  }
  return grid[y][x];
}

// this is where we can add stuff for interacting with map icons
bool Map::is_passable(int x, int y, const Hero &hero) const {
  char tile = get_tile(x, y);

  // Floor tiles are always passable
  if (tile == '.' || tile == ' ')
    return true;

  // water
  if (tile == '~')
    return false;

  // walls
  if (tile == '#')
    return false;

  // default to true for everything else so we dont get stuck or somethin
  return true;
}
