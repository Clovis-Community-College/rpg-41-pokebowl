#ifndef LLBRIDGES_H
#define LLBRIDGES_H

#include <cstdint>

class Actor;

class HasInitiative {
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete to manage speed as needed, just don't touch the
	speed() interface
	*/
	int8_t _speed;
public:
	// Initial speed:
	// actor->start_speed() holds the initial speed valur for each actor
	// use that then add on the dice
	decltype(_speed) speed(Actor* actor);
};

#endif
