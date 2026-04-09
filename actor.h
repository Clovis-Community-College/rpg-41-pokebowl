#ifndef ACTOR_H
#define ACTOR_H

#include <functional>
#include <string>
#include <iostream>
#include "llbridges.h"

using std::string, std::cout, std::cin, std::function;
using HP = int32_t;

// Map could have positive coords or negative-positive coords
// int32_t for flexibility
struct XY {
	int32_t x, y;
};

enum Direction {
	LEFT, RIGHT, UP, DOWN,
	LEFT_UP, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN
	// TBD: combination, varying steps per press
};

class Actor {
private:
	// Name of actor
	string _name;

	// Position of actor
	XY _pos;

	// HP of actor. Should default to HP_MAX.
	// Invariant: 0 <= hp <= HP_MAX, no negative hp
	// unless processing the score itself
	HP _hp;

protected:
	constexpr static int32_t HP_MAX = INT32_MAX;

	// setter with validation
	void name(string _name_);
	void pos(XY _pos_);
	void hp(HP _hp_);

public:
	// Cstor
	Actor(string init_name);
	Actor(string init_name, XY init_xy, HP init_hp);

	// Get (no set)
	string name() const;
	XY pos() const;
	HP hp() const; 

	// Move behaviour. TBI by subclasses.
	// Actor should only move on int32_teger-based steps
	// (i.e., actors on a chessboard).
	virtual void move(Direction d) = 0;

	// Do damage to another Actor.
	// Defaults to 'delta' damage, impl by subclass
	virtual void take_damage(HP delta);
};

class Wall : public Actor {
public:
	Wall(XY xy);
	void move(Direction d) override final;
	void take_damage(HP delta) override;
};

class Hero : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	void move(Direction d) override; // !!!!!!!! each hero is unique, so cannot final here
};

// Hero - Hebrew
// internal names only
class Aleph : public Hero {
	using Hero::Hero;
};

class Bet : public Hero {
	using Hero::Hero;
};

class Gimel : public Hero {
	using Hero::Hero;
};

class Dalet : public Hero {
	using Hero::Hero;
};

class He : public Hero {
	using Hero::Hero;
};

class Vav : public Hero {
	using Hero::Hero;
};

class Zayin : public Hero {
	using Hero::Hero;
};

class Chet : public Hero {
	using Hero::Hero;
};

class Monster : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	virtual bool is_boss() const; // default to false
	void move(Direction d) override; // !!!!!!!! each hero is unique, so cannot final here
};

// Monster - Military
// internal names only
class Alpha : public Monster { using Monster::Monster; };
class Bravo : public Monster { using Monster::Monster; };
class Charlie : public Monster { using Monster::Monster; };
class Delta : public Monster { using Monster::Monster; };
class Echo : public Monster { using Monster::Monster; };
class Foxtrot : public Monster {
	using Monster::Monster;
	bool is_boss() const override; 
};
class Golf : public Monster { using Monster::Monster; };
class Hotel : public Monster { using Monster::Monster; };

// TODO: diagram with links to code snippets

#endif
