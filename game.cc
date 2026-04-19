#include "game.h"

Game::Game() {
    h_aleph = new Aleph("Aleph", {0, 1});
    boss_mob = new Alpha("Boss M.", {98, 98});
    h_main = new Bet("Bet", {100, 100});
    h_chet = new Chet("Chet", {101, 2});
    h_dalet = new Dalet("Dalet", {102, 3});

    world.generate();

    player_party.add_member(h_main);
    player_party.add_member(h_aleph);
    player_party.add_member(h_chet);
    player_party.add_member(h_dalet);
    
    player_party.init_history(h_main->pos());

    player_party.shared_inventory.insert(Item("Potion", "Consume", 50, 50, 0, false));
    player_party.shared_inventory.insert(Item("Sword", "Weapon", 500, 0, 20, false));

    boss_mob->items = Inventory();
    boss_mob->items->insert(Item("Boss Key", "Key", 0, 0, 0, true));
    boss_mob->items->insert(Item("Eye", "Material", 10, 0, 0, false));

    state = GameState::MAP;
    inv_sub = InvSubState::BROWSE;
    equip_sub = EquipSubState::SELECT_HERO;
    inv_cursor = 0;
    equip_cursor = 0;
    equip_hero_idx = 0;

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
        init_pair(5, COLOR_WHITE, COLOR_BLACK);
    }
}

Game::~Game() {
    endwin();
    delete h_aleph;
    delete boss_mob;
    delete h_main;
    delete h_chet;
    delete h_dalet;
}

void Game::handle_input(int ch) {
    if (state == GameState::MAP) {
        if (ch == 'i') {
            state = GameState::INVENTORY;
            inv_sub = InvSubState::BROWSE;
            inv_cursor = 0;
            return;
        }
        if (ch == 'e') {
            state = GameState::EQUIPMENT;
            equip_sub = EquipSubState::SELECT_HERO;
            equip_hero_idx = 0;
            equip_cursor = 0;
            return;
        }

        int target_x = h_main->pos().x;
        int target_y = h_main->pos().y;
        Direction dir = UP;
        bool wants_to_move = false;

        switch (ch) {
            case KEY_UP: target_y--; dir = UP; wants_to_move = true; break;
            case KEY_DOWN: target_y++; dir = DOWN; wants_to_move = true; break;
            case KEY_LEFT: target_x--; dir = LEFT; wants_to_move = true; break;
            case KEY_RIGHT: target_x++; dir = RIGHT; wants_to_move = true; break;
        }

        if (wants_to_move) {
            if (target_x == boss_mob->pos().x && target_y == boss_mob->pos().y) {
                state = GameState::COMBAT;
            } else if (target_x >= 0 && target_y >= 0 && target_x < world.get_width() &&
                       target_y < world.get_height() && world.is_passable(target_x, target_y, *h_main)) {
                
                XY old_pos = h_main->pos();
                h_main->move(dir);
                if (h_main->pos().x != old_pos.x || h_main->pos().y != old_pos.y) {
                    player_party.record_move(old_pos);
                }
            }
        }
    } else if (state == GameState::INVENTORY) {
        if (inv_sub == InvSubState::BROWSE) {
            std::vector<Item> items;
            player_party.shared_inventory.get_all_items(items);
            if (ch == 'i') {
                state = GameState::MAP;
            } else if (ch == KEY_UP && inv_cursor > 0) {
                inv_cursor--;
            } else if (ch == KEY_DOWN && inv_cursor < (int)items.size() - 1) {
                inv_cursor++;
            } else if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
                if (!items.empty()) {
                    selected_item = items[inv_cursor];
                    inv_sub = InvSubState::EQUIP;
                }
            }
        } else if (inv_sub == InvSubState::EQUIP) {
            if (ch == 27 || ch == 'i') {
                inv_sub = InvSubState::BROWSE;
            } else if (ch >= '1' && ch <= '0' + (int)player_party.bank.size()) {
                int hero_idx = ch - '1';
                player_party.shared_inventory.remove(selected_item.get_name());
                
                std::string type = selected_item.get_type();
                if (type == "Weapon" || type == "Armor" || type == "Equipment") {
                    if (!player_party.bank[hero_idx]->items.has_value()) {
                        player_party.bank[hero_idx]->items = Inventory();
                    }
                    player_party.bank[hero_idx]->items->insert(selected_item);
                } else {
                    player_party.bank[hero_idx]->cure_damage(selected_item.get_heal(), 1.0f);
                }

                inv_sub = InvSubState::BROWSE;
                inv_cursor = 0;
            }
        }
    } else if (state == GameState::EQUIPMENT) {
        if (equip_sub == EquipSubState::SELECT_HERO) {
            if (ch == 'e' || ch == 27) {
                state = GameState::MAP;
            } else if (ch >= '1' && ch <= '0' + (int)player_party.bank.size()) {
                equip_hero_idx = ch - '1';
                equip_sub = EquipSubState::VIEW_HERO;
                equip_cursor = 0;
            }
        } else if (equip_sub == EquipSubState::VIEW_HERO) {
            if (ch == 27 || ch == 'e') {
                equip_sub = EquipSubState::SELECT_HERO;
            } else {
                std::vector<Item> equipped;
                if (player_party.bank[equip_hero_idx]->items.has_value()) {
                    player_party.bank[equip_hero_idx]->items->get_all_items(equipped);
                }
                if (ch == KEY_UP && equip_cursor > 0) {
                    equip_cursor--;
                } else if (ch == KEY_DOWN && equip_cursor < (int)equipped.size() - 1) {
                    equip_cursor++;
                } else if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
                    if (!equipped.empty()) {
                        Item to_unequip = equipped[equip_cursor];
                        player_party.bank[equip_hero_idx]->items->remove(to_unequip.get_name());
                        player_party.shared_inventory.insert(to_unequip);
                        if (equip_cursor > 0) equip_cursor--;
                    }
                }
            }
        }
    } else if (state == GameState::COMBAT) {
        if (ch == 'k') {
            std::vector<Item> drops;
            if (boss_mob->items.has_value()) {
                boss_mob->items->get_all_items(drops);
                for (const auto& item : drops) {
                    player_party.shared_inventory.insert(item);
                }
            }
            state = GameState::MAP;
            boss_mob->pos({-1, -1});
        } else if (ch == 'r') {
            state = GameState::MAP;
            if (!player_party.history.empty()) {
                h_main->pos(player_party.history[0]);
            }
        }
    }
}

void Game::render() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    clear();

    int start_y = h_main->pos().y - max_y / 2;
    int start_x = h_main->pos().x - max_x / 2;

    if (state == GameState::MAP || state == GameState::INVENTORY || state == GameState::EQUIPMENT) {
        for (int y = 0; y < max_y; ++y) {
            for (int x = 0; x < max_x; ++x) {
                int map_x = start_x + x;
                int map_y = start_y + y;
                char tile = world.get_tile(map_x, map_y);
                if (tile == '~') {
                    attron(COLOR_PAIR(3)); mvaddch(y, x, tile); attroff(COLOR_PAIR(3));
                } else if (tile == '.') {
                    attron(COLOR_PAIR(1)); mvaddch(y, x, tile); attroff(COLOR_PAIR(1));
                } else {
                    attron(COLOR_PAIR(4)); mvaddch(y, x, tile); attroff(COLOR_PAIR(4));
                }
            }
        }

        int mx = boss_mob->pos().x - start_x;
        int my = boss_mob->pos().y - start_y;
        if (mx >= 0 && mx < max_x && my >= 0 && my < max_y) {
            attron(COLOR_PAIR(2)); mvaddch(my, mx, 'M'); attroff(COLOR_PAIR(2));
        }

        if (state == GameState::MAP) {
            weather.Update(world, h_main->pos());
            weather.draw(h_main->pos().x - start_x, h_main->pos().y - start_y, max_x, max_y);
        }

        char p_chars[] = {'@', 'A', 'B', 'C', 'D', 'E'};
        for (int i = (int)player_party.bank.size() - 1; i >= 0; i--) {
            int px = player_party.bank[i]->pos().x - start_x;
            int py = player_party.bank[i]->pos().y - start_y;
            if (px >= 0 && px < max_x && py >= 0 && py < max_y) {
                int color = (i == 0) ? 1 : 4;
                attron(COLOR_PAIR(color)); mvaddch(py, px, p_chars[i]); attroff(COLOR_PAIR(color));
            }
        }

        if (state == GameState::INVENTORY) {
            std::vector<Item> items;
            player_party.shared_inventory.get_all_items(items);

            attron(COLOR_PAIR(5));
            for(int i = 2; i < max_y - 2; i++) mvprintw(i, 2, "                                                               ");
            mvprintw(2, 4, "=== PARTY INVENTORY ===");
            
            if (items.empty()) {
                mvprintw(4, 4, "Your inventory is empty!");
            } else {
                for (size_t k = 0; k < items.size(); ++k) {
                    if ((int)k == inv_cursor && inv_sub == InvSubState::BROWSE) attron(A_REVERSE);
                    mvprintw(4 + k, 4, "%s - %s", items[k].get_name().c_str(), items[k].get_type().c_str());
                    if ((int)k == inv_cursor && inv_sub == InvSubState::BROWSE) attroff(A_REVERSE);
                }
            }

            if (inv_sub == InvSubState::EQUIP) {
                std::string t = selected_item.get_type();
                bool is_equip = (t == "Weapon" || t == "Armor" || t == "Equipment");

                if (is_equip) {
                    mvprintw(max_y - 8, 4, "!! Equipping: %s", selected_item.get_name().c_str());
                    mvprintw(max_y - 7, 4, "Select hero to equip to:");
                } else {
                    mvprintw(max_y - 8, 4, "!! Using: %s", selected_item.get_name().c_str());
                    mvprintw(max_y - 7, 4, "Select hero to use on:");
                }
                
                for (size_t i = 0; i < player_party.bank.size(); ++i) {
                    mvprintw(max_y - 6 + i, 6, "[%ld] %s", i + 1, player_party.bank[i]->name().c_str());
                }
                
                if (is_equip) {
                    mvprintw(max_y - 2, 4, "Press number to equip, or ESC to cancel.");
                } else {
                    mvprintw(max_y - 2, 4, "Press number to use item, or ESC to cancel.");
                }
            } else {
                mvprintw(max_y - 3, 4, "Up/Down: Browse | Enter: Use/Equip Item | i: Close");
            }
            attroff(COLOR_PAIR(5));
        }

        if (state == GameState::EQUIPMENT) {
            attron(COLOR_PAIR(5));
            for(int i = 2; i < max_y - 2; i++) mvprintw(i, 2, "                                                               ");
            
            if (equip_sub == EquipSubState::SELECT_HERO) {
                mvprintw(2, 4, "=== EQUIPMENT: SELECT HERO ===");
                for (size_t i = 0; i < player_party.bank.size(); ++i) {
                    mvprintw(4 + i, 6, "[%ld] %s", i + 1, player_party.bank[i]->name().c_str());
                }
                mvprintw(max_y - 3, 4, "Press number to select, or 'e'/ESC to close.");
            } else if (equip_sub == EquipSubState::VIEW_HERO) {
                Actor* hero = player_party.bank[equip_hero_idx];
                mvprintw(2, 4, "=== EQUIPMENT: %s ===", hero->name().c_str());
                mvprintw(4, 4, "Stats:");
                mvprintw(5, 6, "HP: %d   Speed: %d   Attack: %d", hero->hp(), hero->starting_speed(), hero->attack_damage());
                
                mvprintw(7, 4, "Equipped Items:");
                std::vector<Item> equipped;
                if (hero->items.has_value()) {
                    hero->items->get_all_items(equipped);
                }
                
                if (equipped.empty()) {
                    mvprintw(9, 6, "No items equipped.");
                } else {
                    for (size_t k = 0; k < equipped.size(); ++k) {
                        if ((int)k == equip_cursor) attron(A_REVERSE);
                        mvprintw(9 + k, 6, "%s - %s", equipped[k].get_name().c_str(), equipped[k].get_type().c_str());
                        if ((int)k == equip_cursor) attroff(A_REVERSE);
                    }
                }
                
                mvprintw(max_y - 3, 4, "Up/Down: Browse | Enter: Unequip Item | ESC/e: Back");
            }
            attroff(COLOR_PAIR(5));
        }

    } else if (state == GameState::COMBAT) {
        mvprintw(2, max_x / 2 - 8, "=== BATTLE OVERVIEW ===");
        
        for (size_t i = 0; i < player_party.bank.size(); ++i) {
             mvprintw(6 + i * 2, 4, "@ %s (HP: %d/%d)", player_party.bank[i]->name().c_str(), player_party.bank[i]->hp(), player_party.bank[i]->hp());
        }

        mvprintw(6, max_x - 40, "M %s (HP: %d/%d)", boss_mob->name().c_str(), boss_mob->hp(), boss_mob->hp());
        mvprintw(8, max_x - 40, "[Boss has drops: %d]", boss_mob->items.has_value() ? boss_mob->items->get_size() : 0);

        mvprintw(max_y - 4, 4, "Battle Mockup - Press 'k' to Kill & Loot Boss");
        mvprintw(max_y - 3, 4, "Press 'r' to Retreat to Map");
    }

    refresh();
}

void Game::run() {
    int ch = 0;
    while (ch != 'q') {
        render();
        ch = getch();
        if (ch == 'q') break;
        handle_input(ch);
    }
}
