#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <deque>
#include "actor.h"
#include "inventory.h"
#include "circularll.h"
#include "weather.h"

class Party {
	WeatherSystem weather(); // tbd: WHEN to change weather? either way Orcs will crawll weathersystem THEN applies it to each Actor.
public:
	// party.bank for the actor vector
	Bank bank;
	CLL turn_order;
	std::deque<XY> history;
	Inventory shared_inventory; // IOrphan pending

	void add_member(Actor* actor);
	void init_history(XY initial_pos);
	void record_move(XY old_pos);

	void kill(Actor* actor, bool gen_drop);

	bool side_dead(ActorType type) const;
	static float weather_scale(string weather); // no need to import weather, as the used thing is only a string!

	// the one ultimate
	// Party-inator
	void inator();
	void you_spin_me_round();
};

#endif
