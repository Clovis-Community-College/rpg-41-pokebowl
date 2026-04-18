#include "actor.h"
#include "llbridges.h"

// Traits
Traits::Traits(AttackHP _do, InversedDefenseScale _hs, Speed _ss, HP _hpm, WeatherScale _wsd) 
	: hp_max(_hpm), attack_damage(_do), hurt_scale(_hs), starting_speed(_ss), weather_scale_damage(_wsd) {}

// Actor
Actor::Actor(string init_name, XY init_xy, HP init_hp, Traits init_traits)
	: _name(init_name), _pos(init_xy), _hp(init_hp), _traits(init_traits), items(nullopt) {}

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

Actor& Actor::operator=(const Actor& actor) {
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
//decltype(Actor::_inv) Actor::items() const { return _inv; }
decltype(Actor::_traits.weather_scale_damage) Actor::weather_scale_damage() const {
	return _traits.weather_scale_damage;
}


bool Actor::is_dead() const { return (_hp <= 0); }

bool Actor::is_self_with(const Actor *other) const {
	return this == other;
}

bool Actor::share_type_with(const Actor *other) const {
	return type() == other->type();
}

void Actor::name(string _name_) { _name = _name_; }
void Actor::pos(XY _pos_) { _pos = _pos_; }

void Actor::weather_scale_damage(WeatherScale wsd) {
	_traits.weather_scale_damage = wsd;
}

void Actor::hp(HP hp_delta, float external_scale, const function<float(float, float)> &op) {
	if (hp_delta < 0) { std::cerr << "hp_delta less than 0"; exit(EXIT_FAILURE); }
	float combined_hp_delta = hp_delta * external_scale * _traits.hurt_scale;
	float new_hp = op((float)hp(), combined_hp_delta);

	[&] (HP _hp_) {
	if (_hp_ > _traits.hp_max)
		_hp = _traits.hp_max; // cap HP

	// -1 acts as a DEAD status code. potentially buggy.
	// can also use 0 for DEAD signal, if death procressing requires 2 frames
	else if (_hp_ < 0)
		_hp = -1;

	else
		_hp = _hp_;
	} (new_hp); // type narrowing intended
	// bcs in here, the needed rounding op is (arbitrarily chosen to be) std::floor()
}

void Actor::take_damage(HP hp_delta, float external_damage_scale = 1) {
	auto op = [](float a, float b) -> float {
		return a - b;
	};
	hp(hp_delta, external_damage_scale, op);
}

void Actor::cure_damage(HP hp_delta, float external_heal_scale = 1) {
	auto op = [](float a, float b) -> float {
		return a + b;
	};
	hp(hp_delta, external_heal_scale, op);
}

void Actor::move(Direction d) {}

void Actor::attack(Actor* opponent) {
	if (!opponent) return; // Null check. Just in case.

	// Condition check
	bool gtg_general = _good_to_attack(opponent); // GENERAL conditions for attack
	bool gtg_specific = _subclass_good_to_attack(opponent); // SUBCLASS-SPECIFIC conditions for attack
	if (!gtg_general || !gtg_specific) return;

	// now opponent ALWAYS lives and not itself
	_attack(opponent);
	
	// plug-in for custom attack behavior
	_post_attack(opponent);
}

bool Actor::_good_to_attack(Actor* opponent) const {
	// Cant attack oneself
	if (is_self_with(opponent)) return false;

	// Cant attack a corpse
        if (opponent->is_dead()) return false;

	// if opponent die, we leave its corpse there, no deletion
	// we can revive it like a zombie????????? hypothetical
	// if so move into subclass version
	return true;
}

bool Actor::_subclass_good_to_attack(Actor* opponent) const {
	// default behavior is to let all attack thru
	// (after general check)
	return true;
}

void Actor::_attack(Actor* opponent) {
	HP hp_delta = attack_damage();
	float external_damage_scale = weather_scale_damage() * 1; // 1 is placehoder for potion effect if have time
	opponent->take_damage(hp_delta, external_damage_scale);
}

void Actor::_post_attack(Actor* opponent) {
	// for subclasses
	// defaults to nothing
}

// Wall
Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX, Traits(0, 0, 0, HP_MAX, 1)) {}

ActorType Wall::type() const { return "wall"; }

void Wall::_attack(Actor* opponent) {
	// no attack is possible if wall attacks sth
}

// Merchant

void Merchant::_attack(Actor* opponent) {
	// no attack is possible if wall attacks sth
	// TODO: open merchant shop and shit
}

ActorType Merchant::type() const { return "merchant"; }
// Drop

Drop::Drop(XY xy, IOrphan io) : Wall(xy), orphaned_items(io) {}

void Drop::_attack(Actor* opponent) {
	// no attack is possible if wall attacks sth
	// TODO: wait for Joe's pickup func
}

ActorType Drop::type() const { return "drop"; }

// NonWall

// template method not useless
// prevention - in case actor subgroup can be refactored
void NonWall::special(Bank& bank) {
	subclass_special(bank);
}


bool NonWall::_subclass_good_to_attack(Actor* opponent) const {
	// no attack of same type - can be overriden
	bool two_actors_different_kind = (type() != opponent->type());
	return two_actors_different_kind;
}

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
Aleph::Aleph(string _name_, XY _pos_) : Hero(_name_, _pos_, 750, Traits(80, 0.75, 1, 750, 1)) {
	// Snorlax-like
	// very high HP, mid attk, slightly worse defense BUT very slow speed, go last
}

Bet::Bet(string _name_, XY _pos_) : Hero(_name_, _pos_, 100, Traits(200, 1.75, 8, 100, 1)) {
	// glass cannon
}

Gimel::Gimel(string _name_, XY _pos_) : Hero(_name_, _pos_, 200, Traits(60, 1, 19, 200, 1)) {
	// speedster - but "speed"-first not speed-first
}

Dalet::Dalet(string _name_, XY _pos_) : Hero(_name_, _pos_, 400, Traits(20, 0.75, 10, 400, 1)) {
	// tbd: healer
}

He::He(string _name_, XY _pos_) : Hero(_name_, _pos_, 300, Traits(100, 1, 13, 300, 1)) {
	// eevee(TM), the most "generic" Poke ever

}

Vav::Vav(string _name_, XY _pos_) : Hero(_name_, _pos_, 250, Traits(150, 1.3, 12, 250, 1)) {
	// berserker
	// with great attack comes great fragility
}

Zayin::Zayin(string _name_, XY _pos_) : Hero(_name_, _pos_, 750, Traits(20, 0.4, 3, 750, 1)) {
	// Shuckle
	// movable wall basically
}

Chet::Chet(string _name_, XY _pos_) : Hero(_name_, _pos_, 180, Traits(180, 1.2, 16, 180, 1)) {
	// Assasin
}

void Aleph::subclass_special(Bank& bank) {
	for (Actor* opponent : bank) {
		// workin with ptrs here
		if (!opponent) continue;
		if (opponent->type() == "monster") this->attack(opponent);
	}
}

void Bet::subclass_special(Bank& bank) {}
void Gimel::subclass_special(Bank& bank) {}


bool Dalet::_subclass_good_to_attack(Actor* opponent) const {
	// no attack of same type
	bool two_actors_different_kind = (type() != opponent->type());
	bool is_hero = opponent->type() == "hero";
	return two_actors_different_kind || is_hero; // logically always true, but semantically makes sense

}

void Dalet::subclass_special(Bank& bank) {}
void He::subclass_special(Bank& bank) {}
void Vav::subclass_special(Bank& bank) {}
void Zayin::subclass_special(Bank& bank) {}
void Chet::subclass_special(Bank& bank) {}

// Monster
ActorType Monster::type() const { return "monster"; }

bool Monster::is_boss() const { return false; }

// M sub
bool Foxtrot::is_boss() const { return true; }

Alpha::Alpha(string _name_, XY _pos_) : Monster(_name_, _pos_, 100, Traits(40, 1.25, 5, 100, 1)) {
	// Rattata
	// alpha version monster, not alpha monster
}

Bravo::Bravo(string _name_, XY _pos_) : Monster(_name_, _pos_, 60, Traits(15, 1, 15, 60, 1)) {
	// Zubat
	// weaker than rattata. but SHOULD BE more numerous?
}

Charlie::Charlie(string _name_, XY _pos_) : Monster(_name_, _pos_, 425, Traits(120, 0.9, 6, 425, 1)) {
	// Machoke
}

Delta::Delta(string _name_, XY _pos_) : Monster(_name_, _pos_, 200, Traits(160, 1.4, 12, 200, 1)) {
	// Gengar, another glass cannon
}

Echo::Echo(string _name_, XY _pos_) : Monster(_name_, _pos_, 300, Traits(100, 1, 10, 300, 1)) {
	// Ditto
}

Foxtrot::Foxtrot(string _name_, XY _pos_) : Monster(_name_, _pos_, 3000, Traits(200, 0.5, 14, 3000, 1)) {
	// BOSS - mewtwo
}

Golf::Golf(string _name_, XY _pos_) : Monster(_name_, _pos_, 400, Traits(20, 0.7, 8, 400, 1)) {
	// Blissey, healer for monsters
}

Hotel::Hotel(string _name_, XY _pos_) : Monster(_name_, _pos_, 220, Traits(140, 1.1, 19, 220, 1)) {
	// Persian
}

void Alpha::subclass_special(Bank& bank) {}
void Bravo::subclass_special(Bank& bank) {}
void Charlie::subclass_special(Bank& bank) {}
void Delta::subclass_special(Bank& bank) {}
void Echo::subclass_special(Bank& bank) {}
void Foxtrot::subclass_special(Bank& bank) {}
void Golf::subclass_special(Bank& bank) {}
void Hotel::subclass_special(Bank& bank) {}
