#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <deque>
#include <stdexcept>
#include "actor.h"
#include "inventory.h"
#include "circularll.h"
#include "weather.h"

enum CombatState {
	init,
	ongoing,
	hero_wins,
	cycle_ends,
	monster_wins
};

class Party {
	WeatherSystem weather(); // tbd: WHEN to change weather? either way Orcs will crawll weathersystem THEN applies it to each Actor.
	uint8_t dead_count = 0;
	uint8_t cycles_left = 2;
	uint8_t monsters = 0, heroes = 0;
public:
	// party.bank for the actor vector
	Bank bank;
	CLL turn_order;
	std::deque<XY> history;
	Inventory shared_inventory; // IOrphan pending

	CombatState status = init;
	string last_action;

	void add_member(Actor* actor);
	void init_history(XY initial_pos);
	void record_move(XY old_pos);

	void post_mortem(Actor* actor, bool gen_drop = true);

//	bool side_dead(ActorType type) const;
	static float weather_scale(string weather); // no need to import weather, as the used thing is only a string!
	void corpse_incinerator(bool forced);

	// the one ultimate
	// Party-inator
	void inator();
	void one_more_time();
};

#endif
