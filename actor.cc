#include "actor.h"
#include "llbridges.h"

Actor::Actor(string init_name, XY init_xy, HP init_hp = HP_MAX) 
	: _name(init_name), _pos(init_xy), _hp(init_hp) {}

string Actor::name() const { return _name; }	

XY Actor::pos() const { return _pos; }

HP Actor::hp() const { return _hp; }

void Actor::name(string _name_) { _name = _name_; }
void Actor::pos(XY _pos_) { _pos = _pos_; }
void Actor::hp(HP _hp_) { _hp = _hp_; }

// move() TBI by subclass
// defauly: 1 step by direction for hero, 2 steps for monster?????
// so it has a "cause" for pure virtual
void Wall::move(Direction d) {}

void Hero::move(Direction d) {}

void Monster::move(Direction d) {}

// Default behaviour: HP damage
void Actor::take_damage(HP delta) { 
	decltype(hp()) new_hp = hp() - delta;
	hp(new_hp);
}

Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX) {}

void Wall::take_damage(HP delta) {
	decltype(hp()) new_hp = hp() - 0; // 0 means no mod
	hp(new_hp);
}

bool Monster::is_boss() const { return false; }

bool Foxtrot::is_boss() const { return true; }
