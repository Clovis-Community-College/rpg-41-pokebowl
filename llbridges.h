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
protected:
	// this is for Actor instances, ala actor._speed
	// this is their FINAL speed after calculation through calling speed()
	int8_t _speed;
public:
	// Initial speed:
	// actor->starting_speed() holds the initial speed valur for each actor
	// use it, then add on the dice onto '_speed'
	decltype(_speed) speed(Actor* actor);
};

#endif
