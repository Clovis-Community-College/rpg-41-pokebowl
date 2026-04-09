#pragma once

#include <cstdint>

class Actor;

class HasInitiative {
private:
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete to manage speed as needed, just don't touch the
	speed() interface
	*/
	int8_t _speed = 1;
public:
	void check_speed(); // checks if speed is within bounds 1-40
	void add_speed(); //will roll d20 and add that to current speed
	void edit_speed(int8_t val); //changes speed to val
	void delete_speed(); //sets speed to 1
	decltype(_speed) speed();
	// delclare here & impl in cc sth like: void speed(Actor* actor);
};
