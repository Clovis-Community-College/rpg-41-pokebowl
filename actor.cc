#include "actor.h"
#include "llbridges.h"

// Traits
Traits::Traits(AttackHP _do, InversedDefenseScale _hs, Speed _ss, HP _hpm) 
	: hp_max(_hpm), attack_damage(_do), hurt_scale(_hs), starting_speed(_ss) {}

// Actor
Actor::Actor(string init_name, XY init_xy, HP init_hp, Traits init_traits)
	: _name(init_name), _pos(init_xy), _hp(init_hp), _traits(init_traits) {}

Actor::Actor(const Actor &actor) : _traits(actor._traits) {
	_name = actor._name;
	_pos = actor._pos;
	_hp = actor._hp;
//	_hp_max = actor._hp_max;
//	_damage_output = actor._damage_output;
//	_starting_speed = actor._starting_speed;
//	_damage_scale = actor._damage_scale;
}

Actor::~Actor() { }

Actor& Actor::operator=(Actor& actor) {
	_name = actor._name;
        _pos = actor._pos;
        _hp = actor._hp;
	_traits = actor._traits;
	return *this;
}

Actor::Actor(Actor&& actor) = default;

Actor& Actor::operator=(Actor&& actor) = default;


string Actor::name() const { return _name; }
XY Actor::pos() const { return _pos; }
HP Actor::hp() const { return _hp; }
decltype(Actor::_traits.starting_speed) Actor::starting_speed() const { return _traits.starting_speed; }
decltype(Actor::_traits.attack_damage) Actor::attack_damage() const { return _traits.attack_damage; }

bool Actor::is_dead() const { return (_hp <= 0); }

void Actor::heal(HP delta) {
	std::cerr << "pls use take_damage() wpls";
	exit(EXIT_FAILURE);
}

void Actor::name(string _name_) { _name = _name_; }
void Actor::pos(XY _pos_) { _pos = _pos_; }
void Actor::hp(HP _hp_) {
	if (_hp_ > _traits.hp_max)
		_hp = _traits.hp_max; // cap HP

	// -1 acts as a DEAD status code. potentially buggy.
	// can also use 0 for DEAD signal, if death procressing requires 2 frames
	else if (_hp_ < 0)
		_hp = -1;

	else
		_hp = _hp_;
}

// if have time, refactor into one generalized function
void Actor::take_damage(HP hp_delta, float external_damage_scale = 1) {
	if (hp_delta < 0) { std::cerr << "hp_delta for take_damage is less than 0"; exit(EXIT_FAILURE); }
	float combined_hp_delta = hp_delta * external_damage_scale * _traits.hurt_scale;
	HP new_hp = std::floor((float)hp() - combined_hp_delta);
	hp(new_hp);
}

void Actor::cure_damage(HP hp_delta, float external_damage_scale = 1) {
	if (hp_delta < 0) { std::cerr << "hp_delta for cure_damage is less than 0"; exit(EXIT_FAILURE); }
	float combined_hp_delta = hp_delta * external_damage_scale * _traits.hurt_scale;
	HP new_hp = std::floor((float)hp() + combined_hp_delta);
	hp(new_hp);
}

// Wall
Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX, Traits(0, 0, 0, HP_MAX)) {}

ActorType Wall::type() const { return "wall"; }

void Wall::move(Direction d) {}

// Hero
ActorType Hero::type() const { return "hero"; }

void Hero::move(Direction d) {
	XY p = pos();
	switch (d) {
		case UP: p.y--; break;
		case DOWN: p.y++; break;
		case LEFT: p.x--; break;
		case RIGHT: p.x++; break;
		default: break;
	}
	pos(p);
}

// H subs
Aleph::Aleph(string _name_, XY _pos_) : Hero(_name_, _pos_, 750, Traits(80, 0.75, 1, 750)) {
	// Snorlax-like
	// very high HP, mid attk, slightly worse defense BUT very slow speed, go last
}

Bet::Bet(string _name_, XY _pos_) : Hero(_name_, _pos_, 100, Traits(200, 1.75, 8, 100)) {
	// glass cannon
}

Gimel::Gimel(string _name_, XY _pos_) : Hero(_name_, _pos_, 200, Traits(60, 1, 19, 200)) {
	// speedster - but "speed"-first not speed-first
}

Dalet::Dalet(string _name_, XY _pos_) : Hero(_name_, _pos_, 400, Traits(20, 0.75, 10, 400)) {
	// tbd: healer
}

He::He(string _name_, XY _pos_) : Hero(_name_, _pos_, 300, Traits(100, 1, 13, 300)) {
	// eevee(TM), the most "generic" Poke ever

}

Vav::Vav(string _name_, XY _pos_) : Hero(_name_, _pos_, 250, Traits(150, 1.3, 12, 250)) {
	// berserker
	// with great attack comes great fragility
}

Zayin::Zayin(string _name_, XY _pos_) : Hero(_name_, _pos_, 750, Traits(20, 0.4, 3, 750)) {
	// Shuckle
	// movable wall basically
}

Chet::Chet(string _name_, XY _pos_) : Hero(_name_, _pos_, 180, Traits(180, 1.2, 16, 180)) {
	// Assasin
}

// Monster
void Monster::move(Direction d) {}

ActorType Monster::type() const { return "monster"; }

bool Monster::is_boss() const { return false; }

// M sub
bool Foxtrot::is_boss() const { return true; }

Alpha::Alpha(string _name_, XY _pos_) : Monster(_name_, _pos_, 100, Traits(40, 1.25, 5, 100)) {
	// Rattata
	// alpha version monster, not alpha monster
}

Bravo::Bravo(string _name_, XY _pos_) : Monster(_name_, _pos_, 60, Traits(15, 1, 15, 60)) {
	// Zubat
	// weaker than rattata. but SHOULD BE more numerous?
}

Charlie::Charlie(string _name_, XY _pos_) : Monster(_name_, _pos_, 425, Traits(120, 0.9, 6, 425)) {
	// Machoke
}

Delta::Delta(string _name_, XY _pos_) : Monster(_name_, _pos_, 200, Traits(160, 1.4, 12, 200)) {
	// Gengar, another glass cannon
}

Echo::Echo(string _name_, XY _pos_) : Monster(_name_, _pos_, 250, Traits(100, 1, 10, 300)) {
	// Ditto
}

Foxtrot::Foxtrot(string _name_, XY _pos_) : Monster(_name_, _pos_, 3000, Traits(200, 0.5, 14, 3000)) {
	// BOSS - mewtwo
}

Golf::Golf(string _name_, XY _pos_) : Monster(_name_, _pos_, 400, Traits(20, 0.7, 8, 400)) {
	// Blissey, healer for monsters
}

Hotel::Hotel(string _name_, XY _pos_) : Monster(_name_, _pos_, 220, Traits(140, 1.1, 19, 220)) {
	// Persian
}
