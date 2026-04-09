#ifndef ACTOR_H
#define ACTOR_H

#include "llbridges.h"
#include <functional>
#include <iostream>
#include <string>
#include <cmath>

using std::string, std::cout, std::cin, std::function;
using HP = int32_t;

// Map could have positive coords or negative-positive coords
// int32_t for flexibility
struct XY {
	int32_t x, y;
};

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LEFT_UP,
	RIGHT_UP,
	LEFT_DOWN,
	RIGHT_DOWN
	// TBD: combination, varying steps per press
};

class Actor {
private:
	// Name of actor
	string _name;

	// Position of actor
	XY _pos;

	// HP of actor. Should default to HP_MAX.
	// Invariant: 0 <= hp <= HP_MAX, no negative hp (except for -1, DEAD actor)
	HP _hp;

	// Turn order.
	const int8_t _start_speed;

	// Damage modulator.
	// 1 correspond 100% of hp_delta, 0.5 is 50% of hp_delta and so on.
	// Defaults to 1
	const float _internal_damage_scale;

	const HP _hp_max; // yes it is UNinitialized, oh the horror. but its deifned
					  // at cstor so its okay

protected:
	constexpr static int32_t HP_MAX = INT32_MAX;

	// setter with validation
	void name(string _name_);
	void pos(XY _pos_);
	void hp(HP _hp_);

public:
	// Cstor
	Actor(string init_name, XY init_xy, HP init_hp, decltype(_internal_damage_scale) ids = 1, decltype(_start_speed) ss = 1);

	// Get (no set)
	string name() const;
	XY pos() const;
	HP hp() const;
	decltype(_start_speed) start_speed() const;

	// Move behaviour. TBI by subclasses.
	// Actor should only move on int32_teger-based steps
	// (i.e., actors on a chessboard).
	virtual void move(Direction d) = 0;

	// Do damage to another Actor.
	// Defaults to 'delta' damage, impl by subclass
	virtual void take_damage(HP hp_delta, float external_damage_scale);
};

// tbd: add overlayable (like sand or water, that has effect)

class Wall : public Actor {
public:
	Wall(XY xy);
	void move(Direction d) override final;
};

class Hero : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	void move(Direction d)
		override; // !!!!!!!! each hero is unique, so cannot final here
};

// Hero - Hebrew
// internal names only
class Aleph : public Hero {
// Fighter 1, half damage instead of full
public:
	Aleph(string _name_, XY _pos_);
};

class Bet : public Hero {
public:
	Bet(string _name_, XY _pos_);
};

class Gimel : public Hero {
public:
	Gimel(string _name_, XY _pos_);
};

class Dalet : public Hero {
public:
	Dalet(string _name_, XY _pos_);
};

class He : public Hero {
public:
	He(string _name_, XY _pos_);
};

class Vav : public Hero {
public:
	Vav(string _name_, XY _pos_);
};

class Zayin : public Hero {
public:
	Zayin(string _name_, XY _pos_);
};

class Chet : public Hero {
public:
	Chet(string _name_, XY _pos_);
};


// Monster - Military
// internal names only
class Monster : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	virtual bool is_boss() const; // default to false
	void move(Direction d)
		override; // !!!!!!!! each hero is unique, so cannot final here
};

class Alpha : public Monster {
public:
	Alpha(string _name_, XY _pos_);
};

class Bravo : public Monster {
public:
	Bravo(string _name_, XY _pos_);
};

class Charlie : public Monster {
public:
	Charlie(string _name_, XY _pos_);
};

class Delta : public Monster {
public:
	Delta(string _name_, XY _pos_);
};

class Echo : public Monster {
public:
	Echo(string _name_, XY _pos_);
};

class Foxtrot : public Monster {
public:
	Foxtrot(string _name_, XY _pos_);
	bool is_boss() const override;
};

class Golf : public Monster {
public:
	Golf(string _name_, XY _pos_);
};

class Hotel : public Monster {
public:
	Hotel(string _name_, XY _pos_);
};

// TODO: diagram with links to code snippets

#endif
