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
};

#endif
