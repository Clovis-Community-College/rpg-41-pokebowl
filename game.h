#pragma once
#include "quest.h"
#include "actor.h"
#include "map.h"
#include "party.h"
#include "weather.h"
#include "inventory.h"
#include "pokeitems.h"
#include "zone.h"
#include <ncurses.h>
#include <sstream>

enum class GameState { MAP, COMBAT, INVENTORY, EQUIPMENT, INN, INN_DIALOG, MERCHANT_DIALOG, MERCHANT_SHOP };
enum class InvSubState { BROWSE, EQUIP };
enum class EquipSubState { SELECT_HERO, VIEW_HERO };
enum class ShopSubState { SELECT_MODE, BUY, SELL };

class Game {
	int seed;
	int grass_variant(int x, int y, int seed);
public:
	GameState state;
	InvSubState inv_sub;
	EquipSubState equip_sub;
	int inv_cursor;
	int equip_cursor;
	int equip_hero_idx;
	Item selected_item;

	constexpr static auto p = []() -> int {
		srand(time(0));
                double rand_01 = ((double)rand() / RAND_MAX);
                double rand_norm =
                        1.375 + 5.0 / (1.0 + pow(2.7128, 5 * (0.4 - rand_01)));
                return (int)round(std::clamp(rand_norm, 3.0, 6.0));
        };
	
	Inventory merchant_inventory;
	ShopSubState shop_sub;
	int shop_cursor;

	Map world;
	WeatherSystem weather;
	Party player_party;
	/*
		Aleph* h_aleph;
		Bet* h_main;
		Chet* h_chet;
		Dalet* h_dalet;
	*/
	Hero *h_main;
	unordered_map<int, Hero *> heroes;
	QuestSystem quests;
	std::vector<Monster *> roaming_monsters;
	std::vector<Monster *> drops;
	Monster *current_enemy;
	bool current_enemy_is_boss;

	Zone inn_zone;
	XY saved_overworld_pos;
	XY inn_pos;
	bool inn_healed;
	std::vector<Item> combat_loot;

	void spawn_monster(bool is_boss = false,  
		function<int(void)> prob = p,
		int x = -1, int y = -1);

	Game();
	~Game();
	void run();

	void snapshot_combat();

private:
	void splash();
	void handle_input(int ch);
	void render();
};
