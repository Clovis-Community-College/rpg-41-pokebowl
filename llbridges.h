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
	decltype(_speed) speed();
	// declare skeleton here & impl func in cc sth like: return-type-here speed(Actor* actor);
};

#endif
