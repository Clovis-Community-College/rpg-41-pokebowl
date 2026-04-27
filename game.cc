#include "game.h"
#include <ncurses.h>
#include <cstdlib>
#include <cmath>

void Game::splash() {
		string splash = R"(
      ____        __        ____                __
     / __ \____  / /_____  / __ )____ _      __/ /
   / /_/ / __ \/ //_/ _ \/ __  / __ \ | /| / / /
 / ____/ /_/ / ,< /  __/ /_/ / /_/ / |/ |/ / /
/_/    \____/_/|_|\___/_____/\____/|__/|__/_/

)";

	string line;
	string prompt = "Greetings, PokeKerney! Press [SPACE] to start.";
	string credit0 = "Made with ", credit1 = "<3", credit2 = " by Tony, McKay, George, Jovanni, and Dillion.";
	int y, x, x_prompt, x_credit0, x_credit1, x_credit2, y_credit, y_prompt;
	getmaxyx(stdscr, y, x);

	y = (y - 10) / 2;
	x_prompt = (x - prompt.length())/2;
	x_credit0 = (x - (credit0 + credit1 + credit2).length())/2;
	x_credit1 = x_credit0 + credit0.length();
	x_credit2 = x_credit1 + credit1.length();
	
	y_credit = y*2 + 8;
	y_prompt = 0;

	// splash screen cuz why not
	nodelay(stdscr, TRUE);
	for (int i = 0; getch() != ' ' ; i++) {
		int lc = 0;		
		std::istringstream iss(splash);

		while (getline(iss, line)) {
			int random = rand() % 4 + 1;
			attron(COLOR_PAIR(random) | A_BOLD);
			mvprintw(y + lc++, (x - line.length())/2, line.c_str());
			attroff(COLOR_PAIR(random) | A_BOLD);
			line.clear();
		}

		int rnd2 = rand() % 13;
		if (!rnd2 && !y_prompt) {
			y_prompt = (!y_prompt) ? (y + ++lc) : 0;
			mvprintw(y_prompt, x_prompt, prompt.c_str());	
		}

		attron(A_DIM);
		mvprintw(y_credit, x_credit0, credit0.c_str());	
		attron(COLOR_PAIR(2) | A_BLINK);
		mvprintw(y_credit, x_credit1, credit1.c_str());	
		attroff(COLOR_PAIR(2) | A_BLINK);
		mvprintw(y_credit, x_credit2, credit2.c_str());	
		attroff(A_DIM);

		napms(100);
		refresh();

		while (lc-- >= 0) {
			move(y + lc, x);
			clrtoeol();  
		}
	}

	nodelay(stdscr, FALSE);
	clear();
}

Game::Game() {
	int cnt = 0;

	while (cnt < 6) {
		int random = rand() % 16;
		Hero *h;
		XY pos = {100, 100};
		switch (random) {
		case 0:
			h = new Aleph("Sleep Apnea-lax", pos);
			break;
		case 1:
			h = new Bet("Heradot", pos);
			break;
		case 2:
			h = new Gimel("Aloha-monola", pos);
			break;
		case 3:
			h = new Dalet("URanusaur", pos);
			break;
		case 4:
			h = new He("Pawn-mot", pos);
			break;
		case 5:
			h = new Vav("Garchonkk", pos);
			break;
		case 6:
			h = new Zayin("Shucked", pos);
			break;
		case 7:
			h = new Chet("Meh-ninja", pos);
			break;
                case 8:
                        h = new Aleph("Snore-chella", pos);
                        break;
                case 9:
                        h = new Bet("Here-a-loss", pos);
                        break;
                case 10:
                        h = new Gimel("Alamos-la", pos);
                        break;
                case 11:
                        h = new Dalet("Ve-no-sir", pos);
                        break;
                case 12:
                        h = new He("Paw-mid", pos);
                        break;
                case 13:
                        h = new Vav("Gaarrr-chump", pos);
                        break;
                case 14:
                        h = new Zayin("Shuck-L", pos);
                        break;
                case 15:
                        h = new Chet("Leaded-inja", pos);
                        break;
		default:
			h = new Zayin("Shucked", pos);
			break;
		}
		if (!heroes.contains(random)) {
			heroes.insert({random, h});
			cnt++;
		}
	}

	current_enemy = nullptr;
	current_enemy_is_boss = false;

	inn_zone.load_inn();
	saved_overworld_pos = {0, 0};
	inn_pos = {0, 0};
	inn_healed = false;

	world.generate();

	int random;
	while (true) {
		random = rand() % 8;
		if (heroes.contains(random)) {
			h_main = heroes.at(random);
			break;
		}
	}

	// ONLY h_main GETS TO BE THE FIRST TO BE ADDED!!!!!!!!
	// only the firts one gets to control the snake
	// pls dont change or cthulu will arise
	player_party.add_member(h_main);

	for (auto &[index, ptr] : heroes)
		if (index != random)
			player_party.add_member(heroes.at(index));

	spawn_monster(true); // Spawn boss at north
	for (int i = 0; i < 15; i++) {
		spawn_monster(false); // Spawn initial roaming monsters
	}

	player_party.init_history(h_main->pos());

	player_party.shared_inventory.insert(
		Item("Potion", "Consume", 50, 50, 0, false));
	player_party.shared_inventory.insert(
		Item("Sword", "Weapon", 500, 0, 20, false));
	player_party.shared_inventory.add_pokecoins(500);

	merchant_inventory.insert(Item("Mega Heal", "heal", 60, 60, 0, false));
	merchant_inventory.insert(
		Item("Big Healing Potion", "heal", 50, 50, 0, false));
	merchant_inventory.insert(Item("Steel Sword", "weapon", 250, 0, 40, false));
	merchant_inventory.insert(Item("Magic Wand", "weapon", 300, 0, 45, false));

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
		init_pair(6, COLOR_WHITE, COLOR_BLUE);
		init_pair(7, COLOR_BLACK, COLOR_WHITE);
		init_pair(8, COLOR_WHITE, COLOR_BLACK); //mountain
		init_pair(9, COLOR_BLACK, COLOR_WHITE); // sand
		init_pair(10, COLOR_GREEN, COLOR_BLACK); // swamp
		init_pair(11, COLOR_MAGENTA, COLOR_MAGENTA); // purple forest??
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
		if (m)
			delete m;
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
	} while (world.get_tile(x, y) != '.' ||
			 (x >= 90 && x <= 110 && y >= 90 && y <= 110));

	if (std::abs(x - h_main->pos().x) < 5 && std::abs(y - h_main->pos().y) < 5)
		return;

	auto prob = [&]() -> int {
		double rand_01 = ((double)rand() / RAND_MAX);
		double rand_norm =
			0.375 + 3.0 / (1.0 + pow(2.7128, 5 * (0.4 - rand_01)));
		return (int)round(std::clamp(rand_norm, 1.0, 3.0));
	};

	Monster *m = nullptr;

	for (int i = 1; i < prob(); i++) {
		int type = rand() % 16;
		switch (type) {
		case 0:
			m = new Alpha("eRaticate", {x, y});
			break;
		case 1:
			m = new Bravo("Chrome-bat", {x, y});
			break;
		case 2:
			m = new Charlie("imMa-choke", {x, y});
			break;
		case 3:
			m = new Delta("Gen-grene", {x, y});
			break;
		case 4:
			m = new Echo("Am-pee-pom", {x, y});
			break;
		case 5:
			m = new Echo("Am-pee-pom", {x, y});
			break;
		case 6:
			m = new Golf("Bliss-you", {x, y});
			break;
		case 7:
			m = new Hotel("Pearl-sian", {x, y});
			break;
                case 8:
                        m = new Alpha("Gas-ticate", {x, y});
                        break;
                case 9:
                        m = new Bravo("Click-bat", {x, y});
                        break;
                case 10:
                        m = new Charlie("Ma-woke", {x, y});
                        break;
                case 11:
                        m = new Delta("Gen-alpha", {x, y});
                        break;
                case 12:
                        m = new Echo("Eye-pom", {x, y});
                        break;
                case 13:
                        m = new Echo("A-simp-om", {x, y});
                        break;
                case 14:
                        m = new Golf("Glissey", {x, y});
                        break;
                case 15:
                        m = new Hotel("Purr-sona", {x, y});
                        break;
		default:
			m = new Alpha("eRaticate", {x, y});
			break;
		}

		if (m)
			roaming_monsters.push_back(m);
	}
}

void Game::snapshot_combat() {
	def_prog_mode();
	endwin();
	system("clear");
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

		// swamp slowdown
		int rnd_stuck = rand() % 2;
		char stepped = world.get_tile(h_main->pos().x, h_main->pos().y);
		if (stepped == '%' && rnd_stuck) return;

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
			bool entered_combat = false;
			for (auto it = roaming_monsters.begin();
				 it != roaming_monsters.end(); ++it) {
				if ((*it)->pos().x == target_x && (*it)->pos().y == target_y) {
					current_enemy = *it;
					current_enemy_is_boss = current_enemy->is_boss();
					*it = nullptr;
					player_party.add_member(current_enemy);
					state = GameState::COMBAT;
					player_party.last_action.clear();
					player_party.inator();
					entered_combat = true;
				}
			}
			std::erase_if(roaming_monsters, [](const Actor *a) { return !a; });
			if (!entered_combat && target_x >= 0 && target_y >= 0 &&
				target_x < world.get_width() && target_y < world.get_height() &&
				world.is_passable(target_x, target_y, *h_main)) {

				XY old_pos = h_main->pos();
				h_main->move(dir);
				if (h_main->pos().x != old_pos.x ||
					h_main->pos().y != old_pos.y) {
					player_party.record_move(old_pos);
					float rand_norm = static_cast<float>(rand()) /
									  static_cast<float>(RAND_MAX);
					if (rand_norm < 0.04 && roaming_monsters.size() < 25) {
						spawn_monster(false);
					}

					char stepped =
						world.get_tile(h_main->pos().x, h_main->pos().y);
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
				if (type == "Weapon" || type == "Armor" ||
					type == "Equipment" || type == "weapon" || type == "heal") {
					if (!player_party.bank[hero_idx]->items.has_value()) {
						player_party.bank[hero_idx]->items = Inventory();
					}
					player_party.bank[hero_idx]->items->insert(selected_item);
				} else {
					player_party.bank[hero_idx]->cure_damage(
						selected_item.get_heal(), 1.0f);
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
					player_party.bank[equip_hero_idx]->items->get_all_items(
						equipped);
				}
				if (ch == KEY_UP && equip_cursor > 0) {
					equip_cursor--;
				} else if (ch == KEY_DOWN &&
						   equip_cursor < (int)equipped.size() - 1) {
					equip_cursor++;
				} else if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
					if (!equipped.empty()) {
						Item to_unequip = equipped[equip_cursor];
						player_party.bank[equip_hero_idx]->items->remove(
							to_unequip.get_name());
						player_party.shared_inventory.insert(to_unequip);
						if (equip_cursor > 0)
							equip_cursor--;
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
		if (ch == 'm' || ch == 'M') {
			state = GameState::MERCHANT_DIALOG;
			return;
		}

		int tx = inn_pos.x;
		int ty = inn_pos.y;
		bool wants_move = false;

		switch (ch) {
		case KEY_UP:
			ty--;
			wants_move = true;
			break;
		case KEY_DOWN:
			ty++;
			wants_move = true;
			break;
		case KEY_LEFT:
			tx--;
			wants_move = true;
			break;
		case KEY_RIGHT:
			tx++;
			wants_move = true;
			break;
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
			for (auto *a : player_party.bank) {
				if (a && a->type() == "hero" && !a->is_dead()) {
					a->cure_damage(9999, 1.0f);
				}
			}
			inn_healed = true;
		} else if (ch == '2' || ch == 27) {
			state = GameState::INN;
		}
	} else if (state == GameState::MERCHANT_DIALOG) {
		if (ch == '1') {
			state = GameState::MERCHANT_SHOP;
			shop_sub = ShopSubState::SELECT_MODE;
			shop_cursor = 0;
		} else if (ch == '2' || ch == 27 || ch == 'm' || ch == 'M') {
			state = GameState::INN;
		}
	} else if (state == GameState::MERCHANT_SHOP) {
		if (shop_sub == ShopSubState::SELECT_MODE) {
			if (ch == 'b' || ch == 'B') {
				shop_sub = ShopSubState::BUY;
				shop_cursor = 0;
			} else if (ch == 's' || ch == 'S') {
				shop_sub = ShopSubState::SELL;
				shop_cursor = 0;
			} else if (ch == 27 || ch == 'm' || ch == 'M') {
				state = GameState::MERCHANT_DIALOG;
			}
		} else if (shop_sub == ShopSubState::BUY) {
			std::vector<Item> items;
			merchant_inventory.get_all_items(items);
			if (ch == 27 || ch == 'm' || ch == 'M' || ch == 'b' || ch == 'B') {
				shop_sub = ShopSubState::SELECT_MODE;
			} else if (ch == KEY_UP && shop_cursor > 0) {
				shop_cursor--;
			} else if (ch == KEY_DOWN && shop_cursor < (int)items.size() - 1) {
				shop_cursor++;
			} else if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
				if (!items.empty() && shop_cursor < (int)items.size()) {
					player_party.shared_inventory.buy(
						merchant_inventory, items[shop_cursor].get_name());
					if (shop_cursor > 0 &&
						shop_cursor >= merchant_inventory.get_size()) {
						shop_cursor--;
					}
				}
			}
		} else if (shop_sub == ShopSubState::SELL) {
			std::vector<Item> items;
			player_party.shared_inventory.get_all_items(items);
			if (ch == 27 || ch == 'm' || ch == 'M' || ch == 's' || ch == 'S') {
				shop_sub = ShopSubState::SELECT_MODE;
			} else if (ch == KEY_UP && shop_cursor > 0) {
				shop_cursor--;
			} else if (ch == KEY_DOWN && shop_cursor < (int)items.size() - 1) {
				shop_cursor++;
			} else if (ch == '\n' || ch == KEY_ENTER || ch == '\r') {
				if (!items.empty() && shop_cursor < (int)items.size()) {
					player_party.shared_inventory.sell(
						merchant_inventory, items[shop_cursor].get_name());
					if (shop_cursor > 0 &&
						shop_cursor >=
							player_party.shared_inventory.get_size()) {
						shop_cursor--;
					}
				}
			}
		}
	} else if (state == GameState::COMBAT) {
		auto& bank = player_party.bank;
		auto mit = find_if(bank.begin(), bank.end(), [](Actor* a){
			Monster* m = dynamic_cast<Monster*>(a);
			if (!m) return false;
			return (m->is_boss());
		});
		
		auto hdit = count_if(bank.begin(), bank.end(), [](Actor* a){
			Hero* h = dynamic_cast<Hero*>(a);
			if (!h) return false;
			return (!h->is_dead());
		});
		bool hdie = !hdit;
		bool boss_present = mit != bank.end();
		if (player_party.status == init || player_party.status == ongoing) {

			if (ch == 'b') {
				snapshot_combat();
			}

			if (ch == ' ') {
				player_party.one_more_time();
				if (player_party.status == hero_wins) {
					combat_loot.clear();
					for (auto *a : player_party.bank) {
						if (a && a->type() == "monster" && a->is_dead() &&
							a->items.has_value()) {
							auto loot = a->items->drop_all();
							for (auto &item : loot) {
								combat_loot.push_back(item);
								player_party.shared_inventory.insert(item);
							}
						}
					}
					auto completed =
						quests.check_items(player_party.shared_inventory);
					if (!completed.empty()) {
						int max_y, max_x;
						getmaxyx(stdscr, max_y, max_x);

						int box_w = 50;
						int box_h = 6 + (int)completed.size() * 2;
						int box_x = (max_x - box_w) / 2;
						int box_y = (max_y - box_h) / 2;

						for (int fade = 0; fade < 3; fade++) {
							erase();

							if (fade == 0)
								attron(A_BOLD | COLOR_PAIR(3));
							else if (fade == 1)
								attron(COLOR_PAIR(3));
							else
								attron(A_DIM | COLOR_PAIR(3));

							for (int y = box_y; y < box_y + box_h; y++) {
								mvhline(y, box_x, ' ', box_w);
							}

							mvprintw(box_y, box_x, "+");
							mvhline(box_y, box_x + 1, '-', box_w - 2);
							mvprintw(box_y, box_x + box_w - 1, "+");
							mvprintw(box_y + box_h - 1, box_x, "+");
							mvhline(box_y + box_h - 1, box_x + 1, '-',
									box_w - 2);
							mvprintw(box_y + box_h - 1, box_x + box_w - 1, "+");
							for (int y = box_y + 1; y < box_y + box_h - 1;
								 y++) {
								mvprintw(y, box_x, "|");
								mvprintw(y, box_x + box_w - 1, "|");
							}

							std::string title = "QUEST COMPLETE!";
							mvprintw(box_y + 2,
									 box_x + (box_w - (int)title.size()) / 2,
									 "%s", title.c_str());

							int line = box_y + 4;
							for (auto &q : completed) {
								std::string star = "* " + q;
								mvprintw(line,
										 box_x + (box_w - (int)star.size()) / 2,
										 "%s", star.c_str());
								line += 2;
							}

							if (fade == 0)
								attroff(A_BOLD | COLOR_PAIR(3));
							else if (fade == 1)
								attroff(COLOR_PAIR(3));
							else
								attroff(A_DIM | COLOR_PAIR(3));

							refresh();

							if (fade == 0)
								napms(1500);
							else if (fade == 1)
								napms(700);
							else
								napms(300);
						}
					}
				}
			} else if (ch == 'r') {
				state = GameState::MAP;
				if (current_enemy) {
					auto &bank = player_party.bank;
					bank.erase(
						std::remove(bank.begin(), bank.end(), current_enemy),
						bank.end());
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
				for (auto it = player_party.bank.begin();
					 it != player_party.bank.end();) {
					Actor *a = *it;
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
		} else if (player_party.status == monster_wins && (boss_present || hdie)) {
			if (ch == ' ') exit(0);
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
	    } else if ( player_party.status == cycle_ends) {
		
			state = GameState::MAP;
			if (current_enemy) {
				auto &bank = player_party.bank;
				bank.erase(
					std::remove(bank.begin(), bank.end(), current_enemy),
					bank.end());
				roaming_monsters.push_back(current_enemy);
				current_enemy = nullptr;
			}
			if (!player_party.history.empty()) {
				h_main->pos(player_party.history[0]);
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

	if (state == GameState::MAP || state == GameState::INVENTORY ||
		state == GameState::EQUIPMENT) {
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
				} else if (tile == '^') {
					attron(COLOR_PAIR(8));
					mvaddch(y, x, tile);
					attroff(COLOR_PAIR(8));
				} else if (tile == ',') {
					attron(COLOR_PAIR(9));
					mvaddch(y, x, tile);
					attroff(COLOR_PAIR(9));
				} else if (tile == '%') {
					attron(COLOR_PAIR(10) | A_DIM);
					mvaddch(y, x, tile);
					attroff(COLOR_PAIR(10) | A_DIM);
				} else if (tile == 'T') {
					attron(COLOR_PAIR(11) | A_BOLD);
					mvaddch(y, x, tile);
					attroff(COLOR_PAIR(11) | A_BOLD);
				} else {
					attron(COLOR_PAIR(4));
					mvaddch(y, x, tile);
					attroff(COLOR_PAIR(4));
				}
			}
		}

		for (auto m : roaming_monsters) {
			if (!m)
				continue;
			int mx = m->pos().x - start_x;
			int my = m->pos().y - start_y;
			if (mx >= 0 && mx < max_x && my >= 0 && my < max_y) {
				attron(COLOR_PAIR(2));
				mvaddch(my, mx, m->is_boss() ? 'B' : 'M');
				attroff(COLOR_PAIR(2));
			}
		}

		if (state == GameState::MAP) {
			weather.Update(world, h_main->pos());
			// changed grid
			//	weather.draw(h_main->pos().x - start_x, h_main->pos().y -
			// start_y, max_x, max_y);
			weather.draw(max_x, max_y);
			string msg = "The weather is now " + weather.getWeather() + "!";

			int row = max_y - 1;
			int col = max_x - msg.length() - 2;

			attron(COLOR_PAIR(5));
			mvprintw(row, col, "%s", msg.c_str());
			attroff(COLOR_PAIR(5));
			// quest
			quests.draw(max_x);
		}

		char p_chars[] = {'@', '+', '+', '+', '+', '+', '+', '+', '+'};
		for (int i = (int)player_party.bank.size() - 1; i >= 0; i--) {
			if (!player_party.bank[i])
				continue;
			else if (player_party.bank[i]->type() != "hero")
				continue;
			int px = player_party.bank[i]->pos().x - start_x;
			int py = player_party.bank[i]->pos().y - start_y;
			if (px >= 0 && px < max_x && py >= 0 && py < max_y) {
				int color = (i == 0) ? 1 : 4;
				// who gets inserted first
				// get to "control moevment"
				// aka, only a[0] gonan be the snake leader!!!
				// nasty bug made by mckay™ (no hate)
				if (player_party.bank[i]->is_dead()) {
					attron(COLOR_PAIR(color) | A_DIM);
					mvaddch(py, px, '.');
					attroff(COLOR_PAIR(color) | A_DIM);
				} else {
					attron(COLOR_PAIR(color) | ((i == 0) ? A_BLINK : 0));
					mvaddch(py, px, p_chars[i]);
					attroff(COLOR_PAIR(color) | ((i == 0) ? A_BLINK : 0));
				}
			}
		}

		if (state == GameState::INVENTORY) {
			std::vector<Item> items;
			player_party.shared_inventory.get_all_items(items);

			attron(COLOR_PAIR(5));
			for (int i = 2; i < max_y - 2; i++)
				mvprintw(i, 2,
						 "                                                     "
						 "          ");
			mvprintw(2, 4, "=== PARTY INVENTORY ===");

			if (items.empty()) {
				mvprintw(4, 4, "Your inventory is empty!");
			} else {
				for (size_t k = 0; k < items.size(); ++k) {
					if ((int)k == inv_cursor && inv_sub == InvSubState::BROWSE)
						attron(A_REVERSE);
					mvprintw(4 + k, 4, "%s - %s", items[k].get_name().c_str(),
							 items[k].get_type().c_str());
					if ((int)k == inv_cursor && inv_sub == InvSubState::BROWSE)
						attroff(A_REVERSE);
				}
			}

			if (inv_sub == InvSubState::EQUIP) {
				std::string t = selected_item.get_type();
				bool is_equip =
					(t == "Weapon" || t == "Armor" || t == "Equipment" ||
					 t == "weapon" || t == "heal");

				if (is_equip) {
					mvprintw(max_y - 8, 4, "!! Equipping: %s",
							 selected_item.get_name().c_str());
					mvprintw(max_y - 7, 4, "Select hero to equip to:");
				} else {
					mvprintw(max_y - 8, 4, "!! Using: %s",
							 selected_item.get_name().c_str());
					mvprintw(max_y - 7, 4, "Select hero to use on:");
				}

				for (size_t i = 0; i < player_party.bank.size(); ++i) {
					if (player_party.bank[i]->type() == "hero")
						mvprintw(max_y - 6 + i, 6, "[%ld] %s", i + 1,
								 player_party.bank[i]->name().c_str());
				}

				if (is_equip) {
					mvprintw(max_y - 2, 4,
							 "Press number to equip, or ESC to cancel.");
				} else {
					mvprintw(max_y - 2, 4,
							 "Press number to use item, or ESC to cancel.");
				}
			} else {
				mvprintw(max_y - 3, 4,
						 "Up/Down: Browse | Enter: Use/Equip Item | i: Close");
			}
			attroff(COLOR_PAIR(5));
		}

		if (state == GameState::EQUIPMENT) {
			attron(COLOR_PAIR(5));
			for (int i = 2; i < max_y - 2; i++)
				mvprintw(i, 2,
						 "                                                     "
						 "          ");

			if (equip_sub == EquipSubState::SELECT_HERO) {
				mvprintw(2, 4, "=== EQUIPMENT: SELECT HERO ===");
				for (size_t i = 0; i < player_party.bank.size(); ++i) {
					if (player_party.bank[i]->type() == "hero")
						mvprintw(4 + i, 6, "[%ld] %s", i + 1,
								 player_party.bank[i]->name().c_str());
				}
				mvprintw(max_y - 3, 4,
						 "Press number to select, or 'e'/ESC to close.");
			} else if (equip_sub == EquipSubState::VIEW_HERO) {
				Actor *hero = player_party.bank[equip_hero_idx];
				mvprintw(2, 4, "=== EQUIPMENT: %s ===", hero->name().c_str());
				mvprintw(4, 4, "Stats:");
				int total_atk = hero->attack_damage();
				int total_heal = 0;
				if (hero->items.has_value()) {
					std::vector<Item> eq;
					hero->items->get_all_items(eq);
					for (auto &it : eq) {
						total_atk += it.get_damage();
						total_heal += it.get_heal();
					}
				}
				mvprintw(5, 6, "HP: %d   Speed: %d   Attack: %d", hero->hp(),
						 hero->starting_speed(), total_atk);
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
						if ((int)k == equip_cursor)
							attron(A_REVERSE);
						mvprintw(9 + k, 6, "%s - %s",
								 equipped[k].get_name().c_str(),
								 equipped[k].get_type().c_str());
						if ((int)k == equip_cursor)
							attroff(A_REVERSE);
					}
				}

				mvprintw(max_y - 3, 4,
						 "Up/Down: Browse | Enter: Unequip Item | ESC/e: Back");
			}
			attroff(COLOR_PAIR(5));
		}

	} else if (state == GameState::INN || state == GameState::INN_DIALOG ||
			   state == GameState::MERCHANT_DIALOG ||
			   state == GameState::MERCHANT_SHOP) {
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
		if (state == GameState::MERCHANT_SHOP) {
			mvprintw(max_y - 1, 2,
					 "Arrows: Browse | Enter: Confirm | b: Buy | s: Sell | "
					 "ESC/m: Exit Shop");
		} else {
			mvprintw(
				max_y - 1, 2,
				"Arrows: Move | h: Innkeeper | m: Merchant | v: Leave Inn");
		}
		attroff(COLOR_PAIR(5));

		int hp_col = 2;
		int hp_row = offset_y - 2;
		if (hp_row < 0)
			hp_row = 0;
		attron(COLOR_PAIR(5));
		mvprintw(hp_row, hp_col, "Party:");
		for (size_t i = 0; i < player_party.bank.size(); i++) {
			Actor *a = player_party.bank[i];
			if (a && a->type() == "hero") {
				int color = a->is_dead() ? 2 : 1;
				attroff(COLOR_PAIR(5));
				attron(COLOR_PAIR(color));
				mvprintw(hp_row + 1 + (int)i, hp_col, "%s HP: %d",
						 a->name().c_str(), a->hp());
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
		} else if (state == GameState::MERCHANT_DIALOG) {
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

			mvprintw(dy + 2, dx + 3, "Merchant:");
			attroff(COLOR_PAIR(4));

			attron(COLOR_PAIR(5));
			mvprintw(dy + 4, dx + 3, "\"Looking for supplies?\"");
			mvprintw(dy + 6, dx + 3, "[1] See wares (Buy/Sell)");
			mvprintw(dy + 7, dx + 3, "[2] Nevermind");
			attroff(COLOR_PAIR(5));
		} else if (state == GameState::MERCHANT_SHOP) {
			int dw = 60;
			int dh = 20;
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
			attroff(COLOR_PAIR(4));

			attron(COLOR_PAIR(5));
			mvprintw(dy + 1, dx + 2, "=== MERCHANT SHOP ===");
			mvprintw(dy + 2, dx + 2, "Party Pokecoins: %d",
					 player_party.shared_inventory.get_coins());

			if (shop_sub == ShopSubState::SELECT_MODE) {
				mvprintw(dy + 5, dx + 4, "What would you like to do?");
				mvprintw(dy + 7, dx + 6, "[b] Buy items");
				mvprintw(dy + 8, dx + 6, "[s] Sell items");
			} else if (shop_sub == ShopSubState::BUY) {
				mvprintw(dy + 4, dx + 2, "--- BUYING ---");
				std::vector<Item> items;
				merchant_inventory.get_all_items(items);
				if (items.empty()) {
					mvprintw(dy + 6, dx + 4, "Sold out!");
				} else {
					for (size_t k = 0; k < items.size() && k < 12; ++k) {
						if ((int)k == shop_cursor)
							attron(A_REVERSE);
						mvprintw(dy + 6 + k, dx + 4, "%-20s (Cost: %d)",
								 items[k].get_name().c_str(),
								 items[k].get_cost());
						if ((int)k == shop_cursor)
							attroff(A_REVERSE);
					}
				}
			} else if (shop_sub == ShopSubState::SELL) {
				mvprintw(dy + 4, dx + 2, "--- SELLING ---");
				std::vector<Item> items;
				player_party.shared_inventory.get_all_items(items);
				if (items.empty()) {
					mvprintw(dy + 6, dx + 4, "You have nothing to sell!");
				} else {
					for (size_t k = 0; k < items.size() && k < 12; ++k) {
						if ((int)k == shop_cursor)
							attron(A_REVERSE);
						mvprintw(dy + 6 + k, dx + 4, "%-20s (Value: %d)",
								 items[k].get_name().c_str(),
								 items[k].get_cost());
						if ((int)k == shop_cursor)
							attroff(A_REVERSE);
					}
				}
			}
			attroff(COLOR_PAIR(5));
		}

	} else if (state == GameState::COMBAT) {
		mvprintw(2, max_x / 2 - 11, "=== BATTLE OVERVIEW ===");
		int hero_y = 6;
		for (size_t i = 0; i < player_party.bank.size(); ++i) {
			string name = player_party.bank[i]->name();
			if (player_party.bank[i] &&
				player_party.bank[i]->type() == "hero") {
				if (player_party.bank[i]->is_dead()) {
					attron(A_DIM);
					mvprintw(hero_y, 4, "X %s (DEAD)",
					name.c_str());
					attroff(A_DIM);
				} else {
					HP hp = player_party.bank[i]->hp();
					HP hpm = player_party.bank[i]->hp_max();
					float prc = (float) hp / (float) hpm;
					string temp = "@ " + name + " (HP: ";
					int no = to_string(hp).length();
					mvprintw(hero_y, 4, "@ %s (HP: ",
							 name.c_str());
					if (prc > 0.8) attron(COLOR_PAIR(1));
					else if (prc > 0.4) attron(COLOR_PAIR(4));
					else if (prc > 0) attron(COLOR_PAIR(2));
					mvprintw(hero_y, 4+temp.length(), "%d",
							 hp);
					attrset(A_NORMAL);
					mvprintw(hero_y, 4+no+temp.length(), ")");
				}
				hero_y += 2;
			}
		}

		int monster_y = 6;
		for (size_t i = 0; i < player_party.bank.size(); ++i) {
			string name = player_party.bank[i]->name();
			if (player_party.bank[i] &&
				player_party.bank[i]->type() == "monster") {
				if (player_party.bank[i]->is_dead()) {
					attron(A_DIM);
					mvprintw(monster_y, max_x - 40, "X %s (DEAD)",
					player_party.bank[i]->name().c_str());
					attroff(A_DIM);
				} else {
					HP hp = player_party.bank[i]->hp();
					HP hpm = player_party.bank[i]->hp_max();
					float prc = (float) hp / (float) hpm;
					string temp = "M " + name + " (HP: ";
					int no = to_string(hp).length();
					mvprintw(monster_y, max_x - 40, "M %s (HP: ",
							 name.c_str());
					if (prc > 0.5) attron(COLOR_PAIR(1));
					else if (prc > 0.25) attron(COLOR_PAIR(4));
					else if (prc > 0) attron(COLOR_PAIR(2));
					mvprintw(monster_y, max_x - 40+temp.length(), "%d",
							 hp);
					attrset(A_NORMAL);
					mvprintw(monster_y, max_x - 40+no+temp.length(), ")");
				}
				monster_y += 2;
			}
		}
		//		player_party.last_action.clear()
		auto& bank = player_party.bank;
		auto mit = find_if(bank.begin(), bank.end(), [](Actor* a){
			Monster* m = dynamic_cast<Monster*>(a);
			if (!m) return false;
			return (m->is_boss());
		});
		auto hdit = count_if(bank.begin(), bank.end(), [](Actor* a){
			Hero* h = dynamic_cast<Hero*>(a);
			if (!h) return false;
			return (!h->is_dead());
		});
		bool boss_present = mit != bank.end();
		bool hdie = !hdit;
		if (!player_party.last_action.empty()) {
			mvprintw(max_y / 2, 4, "> %s", player_party.last_action.c_str());
		}

		if (player_party.status == init || player_party.status == ongoing) {
			mvprintw(max_y - 4, 4,
					 "Press 'SPACE' to next turn, 'b' to visualize in BRIDGES, or 'r' to Run.");
			player_party.last_action.clear();
		} else if (player_party.status == hero_wins) {
			int loot_start = (monster_y > hero_y ? monster_y : hero_y) + 1;
			player_party.last_action.clear();
			loot_start += 3;
			mvprintw(loot_start, 4, "Victory! Loot collected:");
			if (combat_loot.empty()) {
				mvprintw(loot_start + 1, 6, "(no items)");
			} else {
				for (size_t i = 0; i < combat_loot.size() &&
								   (int)(loot_start + 1 + i) < max_y - 3;
					 ++i) {
					mvprintw(loot_start + 1 + i, 6, "- %s (+%d dmg, +%d heal)",
							 combat_loot[i].get_name().c_str(),
							 combat_loot[i].get_damage(),
							 combat_loot[i].get_heal());
				}
				if ((int)(loot_start + 1 + combat_loot.size()) < max_y - 3) {
					mvprintw(loot_start + 1 + combat_loot.size(), 6,
							 "Total: %d items", (int)combat_loot.size());
				}
			}
			mvprintw(max_y - 2, 4, "Press 'SPACE' to return to map.");
		} else if (player_party.status == monster_wins && (boss_present || hdie)) {
			player_party.last_action.clear();
			int max_y, max_x;
			getmaxyx(stdscr, max_y, max_x);
			int start_y = max_y - 25;
			int start_x = 4;
			mvprintw(start_y - 1, start_x, "All heroes ded. Press 'q' or 'SPACE' to quit game.");
			mvprintw(start_y, start_x, "(boo u Kerny)");
			mvprintw(start_y + 1,  start_x, "   ___                       ___   ");
			mvprintw(start_y + 2,  start_x, "  /   \\                     /   \\  ");
			mvprintw(start_y + 3,  start_x, " /     \\___________________/     \\ ");
			mvprintw(start_y + 4,  start_x, "|   _                       _     |");
			mvprintw(start_y + 5,  start_x, "|  / \\                     / \\    |");
			mvprintw(start_y + 6,  start_x, "| ( @ )       _____       ( @ )   |");
			mvprintw(start_y + 7,  start_x, " \\ \\_/       /     \\       \\_/   / ");
			mvprintw(start_y + 8,  start_x, "  \\         /       \\           /  ");
			mvprintw(start_y + 9,  start_x, "   \\_______/         \\_________/   ");
			mvprintw(start_y + 10, start_x, "           \\_________/             ");
		} else if (player_party.status == cycle_ends) {
			player_party.last_action.clear();
			mvprintw(max_y - 4, 4,
					 "Turns expired. Press 'SPACE' to return to map.");
		} else if (player_party.status == monster_wins) {
			player_party.last_action.clear();
			mvprintw(max_y - 4, 4,
					 "Monsters win. Press 'SPACE' to return to map.");
		}
	}

	refresh();
}

void Game::run() {
	atexit([]{ endwin(); }); // no mess up screen on exit
	splash();
	int ch = 0;
	while (ch != 'q') {
		render();
		ch = getch();
		if (ch == 'q')
			break;
		handle_input(ch);
	}
}
