#ifndef ACTOR_H
#define ACTOR_H

#include "llbridges.h"
#include <functional>
#include <iostream>
#include <string>
#include <cmath>

using std::string, std::cout, std::cin, std::function;
using HP = int32_t;
using Speed = int8_t;
using AttackHP = HP;
using InversedDefenseScale = float;

// Map could have positive coords or negative-positive coords
// int32_t for flexibility
struct XY {
	int32_t x, y;
};

struct Traits {
	// Maximum HP an Actor has
	HP hp_max; // yes it is UNinitialized, oh the horror. but its deifned
	// at cstor so its okay

	// Turn order.
	Speed starting_speed;

	// Attack power. 1 signifies no special, 1.x signifies BOOSTED, 0.5 is meh
	// To be used by combat files to give out damage approriately
	// Defaults to 1.
	AttackHP attack_damage;

	// Hurt scale/modulator.
	// 1 correspond 100% of hp_delta, 0.5 is 50% of hp_delta and so on.
	// Defaults to 1
	// In "defense" terms: 1/ids = defense pts.
	InversedDefenseScale hurt_scale;

	// cstor for struct lol
	Traits(AttackHP _do, InversedDefenseScale _hs, Speed _ss, HP _hpm); 
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

	// Trait points.
	Traits _traits;

protected:
	constexpr static int32_t HP_MAX = INT32_MAX;

	// setter with validation
	void name(string _name_);
	void pos(XY _pos_);
	void hp(HP _hp_); // only for internal HP modification.

public:
	// Cstor
	Actor() = delete;
	Actor(string init_name, XY init_xy, HP init_hp, Traits init_traits = {0,0,0,0});

	// Virtual dstor for the virtual dstor god
	virtual ~Actor();

	// Copy cstor for Party system
	Actor(const Actor& actor);

	// rule of 5 guys, gg
	Actor& operator=(Actor& actor);
	Actor(Actor&& actor);
	Actor& operator=(Actor&& actor);

	// Get (no set)
	string name() const;
	XY pos() const;
	HP hp() const;
	//Get for traits
	decltype(_traits.starting_speed) starting_speed() const;

	bool is_dead() const;

	// Move behaviour. TBI by subclasses.
	// Actor should only move on int32_teger-based steps
	// (i.e., actors on a chessboard).
	virtual void move(Direction d) = 0;

	// Do damage to another Actor.
	// Defaults to 'delta' damage, impl by subclass
	// Secret sauce: do a NEGATIVE delta to "heal" actors.
	virtual void take_damage(HP hp_delta, float external_damage_scale);
};

// tbd: add overlayable (like sand or water that brings about effect)

// Inyernal code for n9n-stationary obj
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

// Snorlax-like
class Aleph : public Hero {
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
