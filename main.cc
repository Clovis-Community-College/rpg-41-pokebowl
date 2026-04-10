#include "actor.h"
#include <ctime> //to seed d20 rolls in speed(). see llbridges.cc
#include <ncurses.h>

int main() {
  srand(time({}));
  Wall w({0, 0});
  Aleph h("hero", {0, 1});
  Alpha m("monster", {0, 2});
  Hero h1("", {0, 0}, 22);

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
  }

  h1.move(RIGHT);

  int ch;
  while ((ch = getch()) != 'q') {
    clear();
    switch (ch) {
    case KEY_UP:
      h1.move(UP);
      break;
    case KEY_DOWN:
      h1.move(DOWN);
      break;
    case KEY_LEFT:
      h1.move(LEFT);
      break;
    case KEY_RIGHT:
      h1.move(RIGHT);
      break;
    }
    // drawing onster here
    attron(COLOR_PAIR(2));
    mvaddch(m.pos().y, m.pos().x, 'M');
    attroff(COLOR_PAIR(2));

    // drawing hero
    attron(COLOR_PAIR(1));
    mvaddch(h1.pos().y, h1.pos().x, '@');
    attroff(COLOR_PAIR(1));

    refresh();
  }

  endwin();
}
