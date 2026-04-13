#include "actor.h"
#include "map.h"
#include <ctime>
#include <ncurses.h>

int main() {
  srand(time({}));
  Wall w({0, 0});
  Aleph h("hero", {0, 1});
  Alpha m("monster", {98, 98});
  Hero h1("", {100, 100}, 22);

  Map world;
  world.generate();

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  }

  // Force an initial draw by simulating a keypress or doing drawing before loop
  // othrwise we gotta have them press a key
  int ch = 0;
  while (ch != 'q') {
    int target_x = h1.pos().x;
    int target_y = h1.pos().y;
    Direction dir = UP;
    bool wants_to_move = false;

    switch (ch) {
    case KEY_UP:
      target_y--;
      dir = UP;
      wants_to_move = true;
      break;
    case KEY_DOWN:
      target_y++;
      dir = DOWN;
      wants_to_move = true;
      break;
    case KEY_LEFT:
      target_x--;
      dir = LEFT;
      wants_to_move = true;
      break;
    case KEY_RIGHT:
      target_x++;
      dir = RIGHT;
      wants_to_move = true;
      break;
    }

    if (wants_to_move) {
      char target_tile = world.get_tile(target_x, target_y);

      // Keep hero inside the map boundaries
      if (target_x >= 0 && target_y >= 0 && target_x < world.get_width() &&
          target_y < world.get_height()) {
        if (world.is_passable(target_x, target_y, h1)) {
          h1.move(dir);
        }
      } else {
        // logic to move to another map later on maybe here
      }
    }

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int start_y = h1.pos().y - max_y / 2;
    int start_x = h1.pos().x - max_x / 2;

    for (int y = 0; y < max_y; ++y) {
      for (int x = 0; x < max_x; ++x) {
        int map_x = start_x + x;
        int map_y = start_y + y;
        char tile = world.get_tile(map_x, map_y);

        if (tile == '~') {
          attron(COLOR_PAIR(3));
          mvaddch(y, x, tile);
          attroff(COLOR_PAIR(3));
        } else if (tile == '.') {
          attron(COLOR_PAIR(1));
          mvaddch(y, x, tile);
          attroff(COLOR_PAIR(1));
        } else {
          attron(COLOR_PAIR(4));
          mvaddch(y, x, tile);
          attroff(COLOR_PAIR(4));
        }
      }
    }

    int mx = m.pos().x - start_x;
    int my = m.pos().y - start_y;
    if (mx >= 0 && mx < max_x && my >= 0 && my < max_y) {
      attron(COLOR_PAIR(2));
      mvaddch(my, mx, 'M');
      attroff(COLOR_PAIR(2));
    }

    int hx = h1.pos().x - start_x;
    int hy = h1.pos().y - start_y;
    attron(COLOR_PAIR(1));
    mvaddch(hy, hx, '@');
    attroff(COLOR_PAIR(1));

    refresh();
    ch = getch();
  }

  endwin();
}
