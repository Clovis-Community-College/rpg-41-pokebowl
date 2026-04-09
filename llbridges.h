#ifndef LLBRIDGES_H
#define LLBRIDGES_H

#include <cstdint>

class Actor;

class HasInitiative {
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete to manage speed as needed.
	(and that includes, but not limited to, return type + param type, amt....)
	just don't rename the speed() interface pls
	*/
	int8_t _speed;
public:
	// Initial speed:
	// actor->start_speed holds the initial speed valur for each actor
	// use that then add on the dice
	decltype(_speed) speed(Actor* actor);
};

#endif
