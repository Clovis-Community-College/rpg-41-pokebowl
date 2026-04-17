#ifndef ACTOR_H
#define ACTOR_H

#include "llbridges.h"
#include "inventory.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cmath>
#include <optional>

using std::string, std::cout, std::cin, std::vector, std::optional, std::function;
using HP = int32_t;
using Speed = int8_t;
using AttackHP = HP;
using InversedDefenseScale = float;
using ActorType = string;
using Bank = std::vector<Actor*>;

// Map could have positive coords or negative-positive coords
// int32_t for flexibility
struct XY {
	int32_t x, y;
};

// for the love of god
// pls dont brace init this thing
// use cstor instead
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

	// Generalzied hp mod by delta.
	void hp(HP hp_delta, float external_scale, const function<float(float, float)> &op);

protected:
	constexpr static int32_t HP_MAX = INT32_MAX;

	// setter with validation
	void name(string _name_);
	void pos(XY _pos_);

public:
	// Inventory. NOW FREE TO GRAB AND STEAL /s
	optional<Inventory> items;

	// Cstor
	Actor() = delete;
	Actor(string init_name, XY init_xy, HP init_hp, Traits init_traits = {0,0,0,0});

	// Virtual dstor for the virtual dstor god
	virtual ~Actor();

	// Copy cstor for Party system
	Actor(const Actor& actor);

	// rule of 5 guys, gg
	Actor& operator=(const Actor& actor);
	Actor(Actor&& actor);
	Actor& operator=(Actor&& actor);

	// Get (no set)
	string name() const;
	XY pos() const;
	HP hp() const;
//	decltype(_inv) items() const;
	
	// type of Wall, Hero, Mob into string	
	virtual ActorType type() const = 0;

	//Get for traits
	decltype(_traits.starting_speed) starting_speed() const;
	decltype(_traits.attack_damage) attack_damage() const;

	bool is_dead() const;
	bool is_self_with(const Actor *other) const;
	bool share_type_with(const Actor *other) const;

	// Move behaviour. TBI by subclasses.
	// Actor should only move on int32_teger-based steps
	// (i.e., actors on a chessboard).
	virtual void move(Direction d);

	// Do damage to Actor.
	// Defaults to 'delta' damage, impl by subclass
	// Secret sauce: do a NEGATIVE delta to "heal" actors.
	virtual void take_damage(HP hp_delta, float external_damage_scale);

	// Add HP to Actor.
	// Defaults to 'delta' damage, impl by subclass
	virtual void cure_damage(HP hp_delta, float external_heal_scale);

	// The dreaded dispatch hell
	// if opponent die during attack, opponent cant attk actor.
	void attack(Actor* opponent);

protected:
	bool _good_to_attack(Actor* opponent) const;
	virtual bool _subclass_good_to_attack(Actor* opponent) const;
	virtual void _attack(Actor* opponent);
	virtual void _post_attack(Actor* opponent);
};

// tbd: add overlayable (like sand or water that brings about effect)

// Inyernal code for n9n-stationary obj
class Wall : public Actor {
public:
	Wall(XY xy);
	ActorType type() const override;
	void _attack(Actor* opponent) override;
};

class Merchant : public Wall {
public:
	using Wall::Wall;

	// no override for type, treat merchant like a wall.

	// open merchant shop
	// no good name here bcs.... inheritance hell
	// but composition will need a Shop class galore
	void _attack(Actor* opponent) override;
};

class NonWall : public Actor {
public:
	using Actor::Actor;

protected:
	bool _subclass_good_to_attack(Actor* opponent) const override;

	// SHARED special-related functionalities
	// Applies one actor's "special functions" 
	// on MULTIPLE actor
	void special(Bank& bank);

	// Special abilities. TBI by subclasses.
	virtual void subclass_special(Bank& bank) = 0;
};

class Hero : public NonWall, public HasInitiative {
public:
	using NonWall::NonWall;
	ActorType type() const override;

	// !!!!!!!! each hero is unique, so cannot final here
	void move(Direction d) override;
};

// Hero - Hebrew
// internal names only

// Snorlax-like
class Aleph : public Hero {
public:
	Aleph(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Bet : public Hero {
public:
	Bet(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Gimel : public Hero {
public:
	Gimel(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Dalet : public Hero {
public:
	Dalet(string _name_, XY _pos_);
	bool _subclass_good_to_attack(Actor* opponent) const override final;
	void subclass_special(Bank& bank) override;
};

class He : public Hero {
public:
	He(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Vav : public Hero {
public:
	Vav(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Zayin : public Hero {
public:
	Zayin(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Chet : public Hero {
public:
	Chet(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

// Monster - Military
// internal names only
class Monster : public NonWall, public HasInitiative {
public:
	using NonWall::NonWall;
	ActorType type() const override;
	virtual bool is_boss() const; // default to false
};

class Alpha : public Monster {
public:
	Alpha(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Bravo : public Monster {
public:
	Bravo(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Charlie : public Monster {
public:
	Charlie(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Delta : public Monster {
public:
	Delta(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Echo : public Monster {
public:
	Echo(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Foxtrot : public Monster {
public:
	Foxtrot(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
	bool is_boss() const override;
};

class Golf : public Monster {
public:
	Golf(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

class Hotel : public Monster {
public:
	Hotel(string _name_, XY _pos_);
	void subclass_special(Bank& bank) override;
};

// TODO: diagram with links to code snippets

#endif
