#include "game.h"
#include <ncurses.h>
#include <cstdlib>

Game::Game() {
    h_aleph = new Aleph("Aleph", {0, 1});
    h_main = new Bet("Bet", {100, 100});
    h_chet = new Chet("Chet", {101, 2});
    h_dalet = new Dalet("Dalet", {102, 3});
1+1;
    current_enemy = nullptr;
    current_enemy_is_boss = false;

    inn_zone.load_inn();
    saved_overworld_pos = {0, 0};
    inn_pos = {0, 0};
    inn_healed = false;

    world.generate();

    player_party.add_member(h_main);
    player_party.add_member(h_aleph);
    player_party.add_member(h_chet);
    player_party.add_member(h_dalet);
    
    spawn_monster(true); // Spawn boss at north
    for (int i = 0; i < 15; i++) {
        spawn_monster(false); // Spawn initial roaming monsters
    }
    
    player_party.init_history(h_main->pos());

    player_party.shared_inventory.insert(Item("Potion", "Consume", 50, 50, 0, false));
    player_party.shared_inventory.insert(Item("Sword", "Weapon", 500, 0, 20, false));

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
	// spauly note:
	// too lazy for a kill_all()
	// vector copy prevents iterator hell
auto _bank = player_party.bank;
    for (const auto actor : _bank) 
	player_party.post_mortem(actor, false);

    for (auto m : roaming_monsters) {
        if (m) delete m;
    }
    roaming_monsters.clear();
}

void Game::spawn_monster(bool is_boss) {
    if (is_boss) {
        roaming_monsters.push_back(new Foxtrot("Boss M.", {100, 10}));
        return;
    }
    
    int x, y;
    do {
        x = rand() % world.get_width();
        y = rand() % world.get_height();
    } while (world.get_tile(x, y) != '.' || (x >= 90 && x <= 110 && y >= 90 && y <= 110)); 
    
    if (std::abs(x - h_main->pos().x) < 5 && std::abs(y - h_main->pos().y) < 5) return;

    int type = rand() % 5;
    Monster* m = nullptr;
    switch(type) {
        case 0: m = new Alpha("Alpha", {x, y}); break;
        case 1: m = new Bravo("Bravo", {x, y}); break;
        case 2: m = new Charlie("Charlie", {x, y}); break;
        case 3: m = new Delta("Delta", {x, y}); break;
        case 4: m = new Echo("Echo", {x, y}); break;
    }
    if (m) roaming_monsters.push_back(m);
}

void Game::snapshot_combat() {
	def_prog_mode();
	endwin();
	player_party.turn_order.output_bridges();

	std::cout << "\nPress ENTER to return to combat\n";
	std::cin.get();

	reset_prog_mode();
	refresh();
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
            bool entered_combat = false;
            for (auto it = roaming_monsters.begin(); it != roaming_monsters.end(); ++it) {
                if ((*it)->pos().x == target_x && (*it)->pos().y == target_y) {
                    current_enemy = *it;
                    current_enemy_is_boss = current_enemy->is_boss();
                    roaming_monsters.erase(it);
                    player_party.add_member(current_enemy);
                    state = GameState::COMBAT;
                    player_party.last_action.clear();
                    player_party.inator();
                    entered_combat = true;
                    break;
                }
            }
            if (!entered_combat && target_x >= 0 && target_y >= 0 && target_x < world.get_width() &&
                       target_y < world.get_height() && world.is_passable(target_x, target_y, *h_main)) {
                
                XY old_pos = h_main->pos();
                h_main->move(dir);
                if (h_main->pos().x != old_pos.x || h_main->pos().y != old_pos.y) {
                    player_party.record_move(old_pos);
                    if (rand() % 20 == 0 && roaming_monsters.size() < 25) {
                        spawn_monster(false);
                    }

                    char stepped = world.get_tile(h_main->pos().x, h_main->pos().y);
                    if (stepped == 'I') {
                        saved_overworld_pos = h_main->pos();
                        inn_pos = inn_zone.get_spawn();
                        inn_healed = false;
                        state = GameState::INN;
                    }
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
                if (type == "Weapon" || type == "Armor" || type == "Equipment"||type=="weapon"||type=="heal") {
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
    } else if (state == GameState::INN) {
        if (ch == 'h') {
            state = GameState::INN_DIALOG;
            return;
        }
        if (ch == 'v') {
            h_main->pos(saved_overworld_pos);
            state = GameState::MAP;
            return;
        }

        int tx = inn_pos.x;
        int ty = inn_pos.y;
        bool wants_move = false;

        switch (ch) {
            case KEY_UP:    ty--; wants_move = true; break;
            case KEY_DOWN:  ty++; wants_move = true; break;
            case KEY_LEFT:  tx--; wants_move = true; break;
            case KEY_RIGHT: tx++; wants_move = true; break;
        }

        if (wants_move && inn_zone.is_passable(tx, ty)) {
            if (inn_zone.is_exit(tx, ty)) {
                h_main->pos(saved_overworld_pos);
                state = GameState::MAP;
            } else {
                inn_pos = {tx, ty};
            }
        }
    } else if (state == GameState::INN_DIALOG) {
        if (ch == '1' && !inn_healed) {
            for (auto* a : player_party.bank) {
                if (a && a->type() == "hero" && !a->is_dead()) {
                    a->cure_damage(9999, 1.0f);
                }
            }
            inn_healed = true;
        } else if (ch == '2' || ch == 27) {
            state = GameState::INN;
        }
    } else if (state == GameState::COMBAT) {
        if (player_party.status == init || player_party.status == ongoing) {
			
			if (ch == 'b') {
				snapshot_combat();
			}

			if (ch == ' ') {
                player_party.one_more_time();
                if (player_party.status == hero_wins) {
                    combat_loot.clear();
                    for (auto* a : player_party.bank) {
                        if (a && a->type() == "monster" && a->is_dead() && a->items.has_value()) {
                            auto loot = a->items->drop_all();
                            for (auto& item : loot) {
                                combat_loot.push_back(item);
                                player_party.shared_inventory.insert(item);
                            }
                        }
                    }
                    auto completed = quests.check_items(player_party.shared_inventory);
                    if (!completed.empty()) {
                        int max_y, max_x;
                        getmaxyx(stdscr, max_y, max_x);

                        int box_w = 50;
                        int box_h = 6 + (int)completed.size() * 2;
                        int box_x = (max_x - box_w) / 2;
                        int box_y = (max_y - box_h) / 2;

                        for (int fade = 0; fade < 3; fade++) {
                            erase();

                            if (fade == 0) attron(A_BOLD | COLOR_PAIR(3));
                            else if (fade == 1) attron(COLOR_PAIR(3));
                            else attron(A_DIM | COLOR_PAIR(3));

                            for (int y = box_y; y < box_y + box_h; y++) {
                                mvhline(y, box_x, ' ', box_w);
                            }

                            mvprintw(box_y, box_x, "+");
                            mvhline(box_y, box_x + 1, '-', box_w - 2);
                            mvprintw(box_y, box_x + box_w - 1, "+");
                            mvprintw(box_y + box_h - 1, box_x, "+");
                            mvhline(box_y + box_h - 1, box_x + 1, '-', box_w - 2);
                            mvprintw(box_y + box_h - 1, box_x + box_w - 1, "+");
                            for (int y = box_y + 1; y < box_y + box_h - 1; y++) {
                                mvprintw(y, box_x, "|");
                                mvprintw(y, box_x + box_w - 1, "|");
                            }

                            std::string title = "QUEST COMPLETE!";
                            mvprintw(box_y + 2, box_x + (box_w - (int)title.size()) / 2, "%s", title.c_str());

                            int line = box_y + 4;
                            for (auto& q : completed) {
                                std::string star = "* " + q;
                                mvprintw(line, box_x + (box_w - (int)star.size()) / 2, "%s", star.c_str());
                                line += 2;
                            }

                            if (fade == 0) attroff(A_BOLD | COLOR_PAIR(3));
                            else if (fade == 1) attroff(COLOR_PAIR(3));
                            else attroff(A_DIM | COLOR_PAIR(3));

                            refresh();

                            if (fade == 0) napms(1500);
                            else if (fade == 1) napms(700);
                            else napms(300);
                        }
                    }
                }
            } else if (ch == 'r') {
                state = GameState::MAP;
                if (current_enemy) {
                    auto& bank = player_party.bank;
                    bank.erase(std::remove(bank.begin(), bank.end(), current_enemy), bank.end());
                    roaming_monsters.push_back(current_enemy);
                    current_enemy = nullptr;
                }
                if (!player_party.history.empty()) {
                    h_main->pos(player_party.history[0]);
                }
            }
        } else if (player_party.status == hero_wins) {
            if (ch == ' ') {
                state = GameState::MAP;
                if (current_enemy_is_boss) {
                    quests.boss_killed();
                }
                for (auto it = player_party.bank.begin(); it != player_party.bank.end();) {
                    Actor* a = *it;
                    if (a && a->type() == "monster") {
                        player_party.turn_order.list_delete(a);
                        it = player_party.bank.erase(it);
                        delete a;
                    } else {
                        ++it;
                    }
                }
                combat_loot.clear();
                current_enemy = nullptr;
                current_enemy_is_boss = false;
            }
        } else if (player_party.status == monster_wins) {
            if (ch == ' ') {
                state = GameState::MAP;
                for (auto it = player_party.bank.begin(); it != player_party.bank.end();) {
                    Actor* a = *it;
                    if (a && a->type() == "monster") {
                        player_party.turn_order.list_delete(a);
                        it = player_party.bank.erase(it);
                        delete a;
                    } else {
                        ++it;
                    }
                }
                current_enemy = nullptr;
                current_enemy_is_boss = false;
                if (!player_party.history.empty()) {
                    h_main->pos(player_party.history[0]);
                }
            }
        }
    }
}

void Game::render() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    erase();

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

        for (auto m : roaming_monsters) {
            if (!m) continue;
            int mx = m->pos().x - start_x;
            int my = m->pos().y - start_y;
            if (mx >= 0 && mx < max_x && my >= 0 && my < max_y) {
                attron(COLOR_PAIR(2)); mvaddch(my, mx, m->is_boss() ? 'B' : 'M'); attroff(COLOR_PAIR(2));
            }
        }

        if (state == GameState::MAP) {
            weather.Update(world, h_main->pos());
           // changed grid
		//	weather.draw(h_main->pos().x - start_x, h_main->pos().y - start_y, max_x, max_y);
			weather.draw(max_x, max_y);
			string msg = "The weather is now " + weather.getWeather() +"!";

			int row = max_y - 1;
			int col = max_x - msg.length() -2;

			attron(COLOR_PAIR(5));
			mvprintw(row,col, "%s", msg.c_str());
			attroff(COLOR_PAIR(5));
			// quest 
			quests.draw(max_x);
        }

        char p_chars[] = {'@', '+', '+', '+', '+', '+', '+', '+', '+'};
        for (int i = (int)player_party.bank.size() - 1; i >= 0; i--) {
		if (!player_party.bank[i]) continue;
		else if (player_party.bank[i]->type() != "hero") continue;
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
                bool is_equip = (t == "Weapon" || t == "Armor" || t == "Equipment"||t =="weapon"||t=="heal");

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
                       if (player_party.bank[i]->type() == "hero")
			mvprintw(4 + i, 6, "[%ld] %s", i + 1, player_party.bank[i]->name().c_str());
                }
                mvprintw(max_y - 3, 4, "Press number to select, or 'e'/ESC to close.");
            } else if (equip_sub == EquipSubState::VIEW_HERO) {
                Actor* hero = player_party.bank[equip_hero_idx];
                mvprintw(2, 4, "=== EQUIPMENT: %s ===", hero->name().c_str());
                mvprintw(4, 4, "Stats:");
                int total_atk = hero->attack_damage();
                int total_heal = 0;
                if (hero->items.has_value()) {
                    std::vector<Item> eq;
                    hero->items->get_all_items(eq);
                    for (auto& it : eq) {
                        total_atk += it.get_damage();
                        total_heal += it.get_heal();
                    }
                }
                mvprintw(5, 6, "HP: %d   Speed: %d   Attack: %d", hero->hp(), hero->starting_speed(), total_atk);
                if (total_heal > 0) {
                    mvprintw(6, 6, "Heal Bonus: +%d", total_heal);
                }
                
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

    } else if (state == GameState::INN || state == GameState::INN_DIALOG) {
        int offset_y = (max_y - inn_zone.get_height()) / 2;
        int offset_x = (max_x - inn_zone.get_width()) / 2;

        for (int y = 0; y < inn_zone.get_height(); y++) {
            for (int x = 0; x < inn_zone.get_width(); x++) {
                ZoneTile t = inn_zone.get_tile(x, y);
                int sy = offset_y + y;
                int sx = offset_x + x;
                if (sy >= 0 && sy < max_y && sx >= 0 && sx < max_x) {
                    attron(COLOR_PAIR(t.color_pair));
                    mvaddch(sy, sx, t.ch);
                    attroff(COLOR_PAIR(t.color_pair));
                }
            }
        }

        int py = offset_y + inn_pos.y;
        int px = offset_x + inn_pos.x;
        if (py >= 0 && py < max_y && px >= 0 && px < max_x) {
            attron(COLOR_PAIR(1) | A_BOLD);
            mvaddch(py, px, '@');
            attroff(COLOR_PAIR(1) | A_BOLD);
        }

        attron(COLOR_PAIR(5));
        mvprintw(max_y - 1, 2, "Arrows: Move | h: Talk to Innkeeper | v: Leave Inn");
        attroff(COLOR_PAIR(5));

        int hp_col = 2;
        int hp_row = offset_y - 2;
        if (hp_row < 0) hp_row = 0;
        attron(COLOR_PAIR(5));
        mvprintw(hp_row, hp_col, "Party:");
        for (size_t i = 0; i < player_party.bank.size(); i++) {
            Actor* a = player_party.bank[i];
            if (a && a->type() == "hero") {
                int color = a->is_dead() ? 2 : 1;
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(color));
                mvprintw(hp_row + 1 + (int)i, hp_col, "%s HP: %d", a->name().c_str(), a->hp());
                attroff(COLOR_PAIR(color));
                attron(COLOR_PAIR(5));
            }
        }
        attroff(COLOR_PAIR(5));

        if (state == GameState::INN_DIALOG) {
            int dw = 40;
            int dh = 10;
            int dx = (max_x - dw) / 2;
            int dy = (max_y - dh) / 2;

            attron(COLOR_PAIR(4));
            for (int y = dy; y < dy + dh; y++) {
                mvhline(y, dx, ' ', dw);
            }
            mvprintw(dy, dx, "+");
            mvhline(dy, dx + 1, '-', dw - 2);
            mvprintw(dy, dx + dw - 1, "+");
            mvprintw(dy + dh - 1, dx, "+");
            mvhline(dy + dh - 1, dx + 1, '-', dw - 2);
            mvprintw(dy + dh - 1, dx + dw - 1, "+");
            for (int y = dy + 1; y < dy + dh - 1; y++) {
                mvprintw(y, dx, "|");
                mvprintw(y, dx + dw - 1, "|");
            }

            mvprintw(dy + 2, dx + 3, "Innkeeper:");
            attroff(COLOR_PAIR(4));

            attron(COLOR_PAIR(5));
            if (inn_healed) {
                mvprintw(dy + 4, dx + 3, "\"Your party looks well-rested!\"");
                mvprintw(dy + 6, dx + 3, "[2] Goodbye");
            } else {
                mvprintw(dy + 4, dx + 3, "\"Weary traveler! Rest here?\"");
                mvprintw(dy + 6, dx + 3, "[1] Rest here (Heal party)");
                mvprintw(dy + 7, dx + 3, "[2] Nevermind");
            }
            attroff(COLOR_PAIR(5));
        }

    } else if (state == GameState::COMBAT) {
        mvprintw(2, max_x / 2 - 11, "=== BATTLE OVERVIEW ===");
        
        int hero_y = 6;
        for (size_t i = 0; i < player_party.bank.size(); ++i) {
            if (player_party.bank[i] && player_party.bank[i]->type() == "hero") {
                if (player_party.bank[i]->is_dead()) {
                    mvprintw(hero_y, 4, "X %s (DEAD)", player_party.bank[i]->name().c_str());
                } else {
                    mvprintw(hero_y, 4, "@ %s (HP: %d)", player_party.bank[i]->name().c_str(), player_party.bank[i]->hp());
                }
                hero_y += 2;
            }
        }

        int monster_y = 6;
        for (size_t i = 0; i < player_party.bank.size(); ++i) {
            if (player_party.bank[i] && player_party.bank[i]->type() == "monster") {
                if (player_party.bank[i]->is_dead()) {
                    mvprintw(monster_y, max_x - 40, "X %s (DEAD)", player_party.bank[i]->name().c_str());
                } else {
                    mvprintw(monster_y, max_x - 40, "M %s (HP: %d)", player_party.bank[i]->name().c_str(), player_party.bank[i]->hp());
                }
                monster_y += 2;
            }
        }

        if (!player_party.last_action.empty()) {
            mvprintw(max_y / 2, 4, "> %s", player_party.last_action.c_str());
        }

        if (player_party.status == init || player_party.status == ongoing) {
            mvprintw(max_y - 4, 4, "Press 'SPACE' to next turn, or 'r' to Run.");
        } else if (player_party.status == hero_wins) {
            int loot_start = (monster_y > hero_y ? monster_y : hero_y) + 1;
            mvprintw(loot_start, 4, "Victory! Loot collected:");
            if (combat_loot.empty()) {
                mvprintw(loot_start + 1, 6, "(no items)");
            } else {
                for (size_t i = 0; i < combat_loot.size() && (int)(loot_start + 1 + i) < max_y - 3; ++i) {
                    mvprintw(loot_start + 1 + i, 6, "- %s (+%d dmg, +%d heal)",
                        combat_loot[i].get_name().c_str(),
                        combat_loot[i].get_damage(),
                        combat_loot[i].get_heal());
                }
                if ((int)(loot_start + 1 + combat_loot.size()) < max_y - 3) {
                    mvprintw(loot_start + 1 + combat_loot.size(), 6, "Total: %d items", (int)combat_loot.size());
                }
            }
            mvprintw(max_y - 2, 4, "Press 'SPACE' to return to map.");
        } else if (player_party.status == monster_wins) {
            mvprintw(max_y - 4, 4, "Monsters win... Press 'SPACE' to return to map.");
        } else if (player_party.status == cycle_ends) {
	    mvprintw(max_y - 4, 4, "Turns expired. Press 'SPACE' to return to map.");
	}
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
