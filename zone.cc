#include "zone.h"
#include "actor.h"

Zone::Zone() : width(0), height(0), spawn{0, 0}, exit_pt{0, 0} {}

void Zone::load_inn() {
    std::vector<std::string> layout = {
        "+========================================+",
        "|          ~ POKEBOWL INN ~              |",
        "+========================================+",
        "|                                        |",
        "|  .-------.  .-------.                  |",
        "|  | = = = |  | = = = |   .----------.  |",
        "|  | = = = |  | = = = |   |  COUNTER |  |",
        "|  '-------'  '-------'   |----------|  |",
        "|                         |    N      |  |",
        "|     c T c      c T c    '----------'   |",
        "|                                        |",
        "|     c T c      c T c    .----------.   |",
        "|                         |  o    o  |   |",
        "|                         | o  OO  o |   |",
        "|  .~.~.~.~.~.~.~.~.     '----------'   |",
        "|  | ^  *  ^  *  ^ |                     |",
        "|  '~'~'~'~'~'~'~'~'                    |",
        "+==================v====================+",
    };

    height = layout.size();
    width = layout[0].size();

    grid.resize(height, std::vector<ZoneTile>(width, {' ', 5}));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < (int)layout[y].size(); x++) {
            char c = layout[y][x];
            int color = 5;

            switch (c) {
                case '+': case '-': case '|': case '.': case '\'':
                    color = 5;
                    break;
                case '=':
                    color = 3;
                    break;
                case 'T':
                    color = 4;
                    break;
                case 'c':
                    color = 4;
                    break;
                case '^': case '*':
                    color = 2;
                    break;
                case '~':
                    color = 2;
                    break;
                case 'N':
                    color = 4;
                    break;
                case 'o': case 'O':
                    color = 4;
                    break;
                case 'v':
                    color = 1;
                    break;
                case ' ':
                    color = 1;
                    break;
                default:
                    color = 5;
                    break;
            }

            grid[y][x] = {c, color};
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x].ch == 'v') {
                exit_pt = {x, y};
            }
        }
    }

    spawn = {exit_pt.x, exit_pt.y - 2};
}

ZoneTile Zone::get_tile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return {'#', 5};
    }
    return grid[y][x];
}

bool Zone::is_passable(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    char c = grid[y][x].ch;
    if (c == ' ') return true;
    if (c == 'v') return true;
    if (c == 'N') return true;
    return false;
}

bool Zone::is_exit(int x, int y) const {
    return x == exit_pt.x && y == exit_pt.y;
}

bool Zone::is_npc(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].ch == 'N';
}

int Zone::get_width() const { return width; }
int Zone::get_height() const { return height; }
XY Zone::get_spawn() const { return spawn; }
