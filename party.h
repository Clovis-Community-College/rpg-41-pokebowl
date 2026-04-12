#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include "actor.h"

// The madhouse class controlling attack

// [X request order] -> Party -> [Y (or func of Y) execute X's order]
// [Y result] -> Party -> [apply Y result for X, possibly Z....]

// Instansiated ONCE per game.
class Party {
public:
	// party.bank for the vector
	std::vector<Actor*> bank;
};

#endif
