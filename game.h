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

enum class GameState { MAP, COMBAT, INVENTORY, EQUIPMENT, INN, INN_DIALOG };
enum class InvSubState { BROWSE, EQUIP };
enum class EquipSubState { SELECT_HERO, VIEW_HERO };

class Game {
public:
    GameState state;
    InvSubState inv_sub;
    EquipSubState equip_sub;
    int inv_cursor;
    int equip_cursor;
    int equip_hero_idx;
    Item selected_item;

    Map world;
    WeatherSystem weather;
    Party player_party;

    Aleph* h_aleph;
    Bet* h_main;
    Chet* h_chet;
    Dalet* h_dalet;
   
	QuestSystem quests;
    std::vector<Monster*> roaming_monsters;
    Monster* current_enemy;
    bool current_enemy_is_boss;

    Zone inn_zone;
    XY saved_overworld_pos;
    XY inn_pos;
    bool inn_healed;
    std::vector<Item> combat_loot;

    void spawn_monster(bool is_boss = false);

    Game();
    ~Game();
    void run();
	
	void snapshot_combat();
private:
    void handle_input(int ch);
    void render();
};
