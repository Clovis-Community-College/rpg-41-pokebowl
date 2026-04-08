#ifndef LLBRIDGES_H
#define LLBRIDGES_H

#include <cstdint>

class Actor;

class HasInitiative {
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete as needed, just don't touch the
	speed() interface
	*/
	Actor* actor;

public:
	int8_t speed();
};

#endif
