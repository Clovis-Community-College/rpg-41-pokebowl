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

class HasInitiative;

class Actor {
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

public:
	// Cstor
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
	// Defaults to no damage.
	virtual void take_damage(HP delta);
};

class Wall : public Actor {
public:
	Wall(XY xy);
	void move(Direction d) override final;
};

class Hero : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	void move(Direction d) override final; // !!!!!!!! each hero is unique, so cannot final here
};

class Monster : public Actor, public HasInitiative {
public:
	using Actor::Actor;
	void move(Direction d) override final; // !!!!!!!! each hero is unique, so cannot final here
};

// TODO: diagram with links to code snippets

#endif
