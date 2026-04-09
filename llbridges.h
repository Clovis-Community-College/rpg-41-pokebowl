#pragma once

#include <cstdint>

class Actor;

class HasInitiative {
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete to manage speed as needed, just don't touch the
	speed() interface
	*/
	int8_t _speed{};
public:
	int8_t get_speed();
	decltype(_speed) speed();
	// delclare here & impl in cc sth like: void speed(Actor* actor);
	void speed(Actor* actor);
};
