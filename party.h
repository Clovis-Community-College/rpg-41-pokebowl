#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include "actor.h"

class Party {
public:
	// party.bank for the vector
	std::vector<Actor*> bank;
};

#endif
