#include "actor.h"
#include "llbridges.h"

// Actor
Actor::Actor(string init_name, XY init_xy, HP init_hp)
	: _name(init_name), _pos(init_xy), _hp(init_hp), _hp_max(init_hp) {}
// HP max is init to current HP (aka all actors has 100% health)

string Actor::name() const { return _name; }
XY Actor::pos() const { return _pos; }
HP Actor::hp() const { return _hp; }

void Actor::name(string _name_) { _name = _name_; }
void Actor::pos(XY _pos_) { _pos = _pos_; }
void Actor::hp(HP _hp_) {
	if (_hp_ > HP_MAX)
		_hp = _hp_max; // cap HP

	// -1 acts as a DEAD status code. potentially buggy.
	// can also use 0 for DEAD signal, if death procressing requires 2 frames
	else if (_hp_ < 0)
		_hp = -1;

	else
		_hp = _hp_;
}

void Actor::take_damage(HP delta) {
	decltype(hp()) new_hp = hp() - delta;
	hp(new_hp);
}

// Wall
Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX) {}

void Wall::move(Direction d) {}

void Wall::take_damage(HP delta) {
	decltype(hp()) new_hp = hp() - 0;
	hp(new_hp);
}

// Hero
void Hero::move(Direction d) {}

// H subs
Aleph::Aleph(string _name_, XY _pos_) : Hero(_name_, _pos_, 500) {} // high HP
Bet::Bet(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
Gimel::Gimel(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
Dalet::Dalet(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
He::He(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
Vav::Vav(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
Zayin::Zayin(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}
Chet::Chet(string _name_, XY _pos_) : Hero(_name_, _pos_, 250) {}

void Aleph::take_damage(HP delta) {
	decltype(hp()) new_hp = hp() - delta/2; // half dam
	hp(new_hp);
}

// Monster
void Monster::move(Direction d) {}

bool Monster::is_boss() const { return false; }

// M sub
bool Foxtrot::is_boss() const { return true; }

Alpha::Alpha(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Bravo::Bravo(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Charlie::Charlie(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Delta::Delta(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Echo::Echo(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Foxtrot::Foxtrot(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Golf::Golf(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
Hotel::Hotel(string _name_, XY _pos_) : Monster(_name_, _pos_, 250) {}
