#include "actor.h"
#include "llbridges.h"

Actor::Actor(string init_name, XY init_xy, HP init_hp = HP_MAX) 
	: _name(init_name), _pos(init_xy), _hp(init_hp) {}

string Actor::name() const { return _name; }	

XY Actor::pos() const { return _pos; }

HP Actor::hp() const { return _hp; }

void name(string _name_);
void pos(XY _pos_);
void hp(HP _hp_);
// move() TBI by subclass

// Default behaviour: HP damage
void Actor::take_damage(HP delta) { 
	delctype(_hp) new_hp = hp() - delta;
	hp(new_hp);
}

Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX) {}

void Wall::take_damage(HP delta) {
	delctype(_hp) new_hp = hp() - 0; // 0 means no mod
	hp(new_hp);
}

