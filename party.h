#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <deque>
#include "actor.h"
#include "inventory.h"

class Party {
public:
	std::vector<Actor*> bank;
    std::deque<XY> history;
    Inventory shared_inventory;

    void add_member(Actor* actor);
    void init_history(XY initial_pos);
    void record_move(XY old_pos);

	bool side_dead(ActorType type) const;
	float weather_scale(string weather) const;
};

#endif
