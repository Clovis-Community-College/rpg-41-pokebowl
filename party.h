#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include "actor.h"

// The madhouse class controlling attack


// Instansiated ONCE per game.
class Party {
public:
	// party.bank for the vector
	std::vector<Actor*> bank;

	bool side_dead(ActorType type) const;
	float weather_scale(string weather) const; // no need to import weather, as the used thing is only a string!
};

#endif
