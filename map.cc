#include "map.h"
#include "actor.h"

Map::Map() : width(200), height(200) {
  grid.resize(height, std::string(width, '.'));
}

void Map::loss(int ox, int oy) {
	int bx = 80, by = 60;
	int pdist = 14, boxdist = 4, inmargin = 10;

	// boudn check. bad => crash
	grid[oy+by][ox+bx] = grid[oy+by][ox+bx];
	grid[oy+by][ox] = grid[oy+by][ox];
	grid[oy][ox+bx] = grid[oy][ox+bx];
	grid[oy][ox] = grid[oy][ox];

	// box 1 2
	for (int y = oy + inmargin/2; y < oy + by/2 - boxdist - inmargin/2; y++) {
		for (int x = ox + inmargin; x < ox + bx/2; x++) {
			if (x == ox + pdist/2 + inmargin) grid[y][x] = '#';
		}
		for (int x = ox + bx/2 + boxdist + inmargin; x <= ox + bx - inmargin; x++) {
			if (x == ox + bx/2 + boxdist + inmargin) grid[y][x] = '#';
			if (y >= oy + 9 && x == ox + bx/2 + inmargin + boxdist + pdist) grid[y][x] = '#';
		}
	}

	// box 3 4
	for (int y = oy + by/2; y <= oy + by - boxdist - inmargin; y++) {
		for (int x = ox + inmargin; x < ox + bx/2; x++) {
			if (x == ox + inmargin || x == ox + inmargin + pdist) grid[y][x] = '#';
			if (x == ox + bx/2 + inmargin + boxdist + pdist) grid[y][x] = '#';
		}
		for (int x = ox + bx/2 + boxdist + inmargin; x <= ox + bx; x++) {
			if (x == ox + bx/2 + boxdist + inmargin) grid[y][x] = '#';
			if (x >= ox + boxdist + inmargin + bx/2 + pdist/2 && y == oy + by - boxdist - inmargin) grid[y][x] = '#';
		}
	}

	// border x-axis
	for (int x = ox; x <= ox + bx + inmargin; x++) grid[oy + by/2 - boxdist][x] = '^';
	
	// border y-axis
	for (int y = oy; y <= oy + by - inmargin; y++) grid[y][ox + bx/2] = '^';

	// monster spawn fest
	for (int y = oy; y <= oy + by; y++) {
		for (int x = ox; x <= ox + bx; x++) {
			int rnd = rand() % 1500;
			if (rnd) continue;
			grid[y][x] = (grid[y][x] == '.') ? 'm' : grid[y][x];
		}
	}
}

void Map::generate() {
  for (int y = 60; y < 80; ++y) {
    for (int x = 90; x < 110; ++x) {
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

 for (int y = 150; y < 180; ++y) {
    for (int x = 60; x < 85; ++x) {
      grid[y][x] = '^';
    }
    for (int x = 90; x < 110; ++x) {
      grid[y][x] = '%';
    }
    for (int x = 110; x < 130; ++x) {
      grid[y][x] = ',';
    }
    for (int x = 130; x < 150; ++x) {
      grid[y][x] = 'T';
    }
  }

  loss(105, 8);
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
  if (tile == '~' || tile == '^')
    return false;

  // walls
  if (tile == '#')
    return false;

  // default to true for everything else so we dont get stuck or somethin
  return true;
}
