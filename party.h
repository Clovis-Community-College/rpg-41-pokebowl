#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <deque>
#include "actor.h"
#include "inventory.h"
#include "weather.h"

class Party {
	WeatherSystem weather(); // tbd: WHEN to change weather? either way Orcs will crawll weathersystem THEN applies it to each Actor.
public:
	std::vector<Actor*> bank;
    std::deque<XY> history;
    Inventory shared_inventory;

    void add_member(Actor* actor);
    void init_history(XY initial_pos);
    void record_move(XY old_pos);

	bool side_dead(ActorType type) const;
	static float weather_scale(string weather); // no need to import weather, as the used thing is only a string!
};

#endif
