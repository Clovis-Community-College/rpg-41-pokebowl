#pragma once

#include <cstdint>

class Actor;

class HasInitiative {
private:
	/*
	Stub interface for Hero and Monster inheritance only
	Feel free to add-edit-delete to manage speed as needed.
	(and that includes, but not limited to, return type + param type, amt....)
	just don't rename the speed() interface pls
	*/
protected:
	// this is for Actor instances, ala actor._speed
	// this is their FINAL speed after calculation through calling speed()
	int8_t _speed = 1;
public:
	// Initial speed:
	// actor->start_speed() holds the initial speed valur for each actor
	// use it, then add on the dice onto '_speed'
  void check_speed(); // checks if speed is within bounds 1-40
	void add_speed(); //will roll d20 and add that to current speed
	void edit_speed(int8_t val); //changes speed to val
	void delete_speed(); //sets speed to 1
	decltype(_speed) speed(Actor* actor);
};
