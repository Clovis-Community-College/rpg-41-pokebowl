#include "map.h"
#include "actor.h"

Map::Map() : width(200), height(200) {
	grid.resize(height, std::string(width, '.'));
}

void Map::ca(char terrain, char comp_terrain, char die_terrain, int die, int live, bool first_pass, int thicc) {
	// copy vector
	vector<string> rmap = grid;
	vector<string> &wmap = grid;

	// default param
	auto dt = [&](int x, int y){ 
		return (die_terrain == ' ') ? rmap[y][x] : die_terrain;
	};
	auto ct = [&](int x, int y){ 
		return (comp_terrain == ' ') ? rmap[y][x] : comp_terrain;
	};


	auto is_border = [&](int x, int y, int thicc = 0) { return (x == thicc || x == width-thicc-1 || y == thicc || y == width-thicc-1); };
	auto outside_border = [&](int x, int y, int thicc = 0){ return (x < thicc || x > width-thicc-1 || y < thicc || y > width-thicc-1); };

	auto count_living_neighbor = [&](int x, int y){
		int count = 0;
		for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (outside_border(x+dx, y+dy)) continue;
			// only consider rmap!!!!!
			if (rmap[y+dy][x+dx] != terrain && rmap[y+dy][x+dx] != dt(x,y)) continue;
			// dont count dead cells lol
			if (rmap[y+dy][x+dx] == dt(x,y)) continue;
			// randomize comp cell
			if (rmap[y+dy][x+dx] == ct(x,y)) count += (!(rand() % 3));
			else count++;
		}
		}
		return count;
	};


	if (!first_pass) goto apply;

	// create noisy data, overriding everything!!!
	for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
		// limit noise to only border or thicc zone
		if (!outside_border(x, y, thicc) && thicc) continue;
		int rnd = rand() % 2;
		if (!rnd) {
			rmap[y][x] = terrain;
			wmap[y][x] = terrain;
		}
	}
	}

apply:
	// apply automata
	for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
		// rule: 			
		// never writes on existing terrain
		if (rmap[y][x] != dt(x,y) && rmap[y][x] != terrain) continue;

		// if water, border of map is always made up of living cells. 
		if (is_border(x, y) && terrain == '~') {
			wmap[y][x] = terrain;
			continue;
		}
		
		// count LIVING neighbor of rmap
		int living_nbr = count_living_neighbor(x, y);

		// killing any cells with less than  living neighbours, 
		if (living_nbr <= die && !outside_border(x,y)) wmap[y][x] = dt(x,y);

		// resurrecting cells with more than  living neighbours.
		else if (living_nbr >= live && !outside_border(x,y)) wmap[y][x] = terrain;
		else if (!outside_border(x,y)) wmap[y][x] = ct(x,y);
	}
	}
}

void Map::loss(int ox, int oy) {
	int bx = 80, by = 60;
	int pdist = 14, boxdist = 4, inmargin = 10;

	// boudn check. bad => crash
	grid[oy + by][ox + bx] = grid[oy + by][ox + bx];
	grid[oy + by][ox] = grid[oy + by][ox];
	grid[oy][ox + bx] = grid[oy][ox + bx];
	grid[oy][ox] = grid[oy][ox];

	// box 1 2
	for (int y = oy + inmargin / 2; y < oy + by / 2 - boxdist - inmargin / 2;
		 y++) {
		for (int x = ox + inmargin; x < ox + bx / 2; x++) {
			if (x == ox + pdist / 2 + inmargin)
				grid[y][x] = '#';
		}
		for (int x = ox + bx / 2 + boxdist + inmargin; x <= ox + bx - inmargin;
			 x++) {
			if (x == ox + bx / 2 + boxdist + inmargin)
				grid[y][x] = '#';
			if (y >= oy + 9 && x == ox + bx / 2 + inmargin + boxdist + pdist)
				grid[y][x] = '#';
		}
	}

	// box 3 4
	for (int y = oy + by / 2; y <= oy + by - boxdist - inmargin; y++) {
		for (int x = ox + inmargin; x < ox + bx / 2; x++) {
			if (x == ox + inmargin || x == ox + inmargin + pdist)
				grid[y][x] = '#';
			if (x == ox + bx / 2 + inmargin + boxdist + pdist)
				grid[y][x] = '#';
		}
		for (int x = ox + bx / 2 + boxdist + inmargin; x <= ox + bx; x++) {
			if (x == ox + bx / 2 + boxdist + inmargin)
				grid[y][x] = '#';
			if (x >= ox + boxdist + inmargin + bx / 2 + pdist / 2 &&
				y == oy + by - boxdist - inmargin)
				grid[y][x] = '#';
		}
	}

	// border x-axis
	for (int x = ox; x <= ox + bx + inmargin; x++)
		grid[oy + by / 2 - boxdist][x] = '^';

	// border y-axis
	for (int y = oy; y <= oy + by - inmargin; y++)
		grid[y][ox + bx / 2] = '^';

	// monster spawn fest
	for (int y = oy; y <= oy + by; y++) {
		for (int x = ox; x <= ox + bx; x++) {
			int rnd = rand() % 1000;
			if (rnd)
				continue;
			grid[y][x] = (grid[y][x] == '.') ? 'm' : grid[y][x];
		}
	}
}

void Map::au(int ox, int oy, int bx, int by, char body, char visor, char border, char visor_border) {
	// boudn check. bad => crash
	grid[oy + by][ox + bx] = grid[oy + by][ox + bx];
	grid[oy + by][ox] = grid[oy + by][ox];
	grid[oy][ox + bx] = grid[oy][ox + bx];
	grid[oy][ox] = grid[oy][ox];

	ox -= 0.3 * bx;

	for (int y = oy; y <= (oy + 0.75 * by); y++) {
		// body main
		for (int x = (ox + 0.4 * bx); x <= (ox + 0.9 * bx); x++) {
			if ((x == (ox + 0.4 * bx) || x == (ox + 0.9 * bx)) || (y == oy || y == (oy + 0.75 * by))) grid[y][x] = border;
			else grid[y][x] = body;
		}
	}

        for (int y = (oy + 0.75 * by); y <= (oy + 1 * by); y++) {
                // left leg
                for (int x = (ox + 0.4 * bx); x <= (ox + 0.65 * bx); x++) {
                        if ((x == (ox + 0.4 * bx) || x == (ox + 0.65 * bx))|| (y == (oy + 1 * by))) grid[y][x] = border;
                        else grid[y][x] = body;
		}

                // right leg
                for (int x = (ox + 0.7 * bx); x <= (ox + 0.9 * bx); x++) {
                        if ((x == (ox + 0.7 * bx) || x == (ox + 0.9 * bx)) || (y == (oy + 1 * by))) grid[y][x] = border;
                        else grid[y][x] = body;
	
			// border blend into rigth leg
			if (y == (oy + 0.75 * by) && (ox + 0.7 * bx) <= x && x <= (ox + 0.75 * bx)) grid[y][x] = border;
		}
        }

        for (int y = (oy + 0.15 * by); y <= (oy + 0.75 * by); y++) {
                // bkpck
                for (int x = (ox + 0.3 * bx); x <= (ox + 0.4 * bx); x++) {
                        if ((x == (ox + 0.3 * bx) || x == (ox + 0.4 * bx)) || (y == (oy + 0.15 * by) || y == (oy + 0.75 * by))) grid[y][x] = border;
                        else grid[y][x] = body;
		}
        }

        for (int y = (oy + 0.15 * by); y <= (oy + 0.35 * by); y++) {
                // visor
                for (int x = (ox + 0.65 * bx); x <= (ox + 1 * bx); x++) {
                        if ((x == (ox + 0.65 * bx) || x == (ox + 1 * bx)) || (y == (oy + 0.15 * by) || y == (oy + 0.35 * by))) grid[y][x] = visor_border;
                        else grid[y][x] = visor;
		}
        }

}

void Map::generate() {
	/*  for (int y = 60; y < 80; ++y) {
		for (int x = 90; x < 110; ++x) {
		  grid[y][x] = '~';
		}
	  }
	*/
	
	// beach
	ca('~', ',', '.', 1, 4, true, 10);
	ca('~', ',', ' ', 4, 4, false, 17);
	ca(',', '.', '.', 1, 3, true, 15);
	ca(',', ' ', ' ', 6, 7, false, 30);
	ca(',', ' ', ' ', 1, 3, false, 40);
	ca(',', ' ', ' ', 5, 7, false, 40);
	ca('~', ' ', ' ', 4, 6, true, 5);
	ca('~', ' ', ' ', 1, 4, false, 30);
	ca('~', ' ', ' ', 3, 6, false, 30);
//	ca('.', ' ', ',', 3, 5, true, 30);
	ca('.', ' ', ' ', 4, 5, false, 30);

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

	/* for (int y = 150; y < 180; ++y) {
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
	*/
	loss(105, 16);
	au(10, 135, 140, 60, '%', '~', '*', ':');
	au(20, 30, 80, 40, ',', 'T', '%', '*');

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
