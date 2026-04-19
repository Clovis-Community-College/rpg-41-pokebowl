#pragma once
#include "quest.h"
#include "actor.h"
#include "map.h"
#include "party.h"
#include "weather.h"
#include "inventory.h"
#include "pokeitems.h"
#include <ncurses.h>

enum class GameState { MAP, COMBAT, INVENTORY, EQUIPMENT };
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
    Foxtrot* boss_mob;

    Game();
    ~Game();
    void run();

private:
    void handle_input(int ch);
    void render();
};
