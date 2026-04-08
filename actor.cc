#include "actor.h"
#include "llbridges.h"

Actor::Actor(string init_name, XY init_xy, HP init_hp = HP_MAX) 
	: _name(init_name), _pos(init_xy), _hp(init_hp) {}

string Actor::name() const { return _name; }	

XY Actor::pos() const { return _pos; }

HP Actor::hp() const { return _hp; }

// move() TBI by subclass

// Default behaviour: no HP damage
// its for the walls
void Actor::take_damage(HP delta) {}
