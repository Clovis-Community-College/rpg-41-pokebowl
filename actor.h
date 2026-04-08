#include <string>
#include <iostream>

using std::string, std::cout, std::cin;
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
	// Name of actor
	string name;

	// Position of actor
	XY pos;

	// HP of actor. Should default to HP_MAX.
	HP hp;

protected:
	constexpr static int32_t HP_MAX = INT32_MAX;

public:
	// Move behaviour. TBI by subclasses.
	// Actor should only move on int32_teger-based steps
	// (i.e., actors on a chessboard).
	virtual void move(Direction d) = 0;

	// Do damage to another Actor.
	// Defaults to no damage.
	virtual void take_damage(HP delta);
};

class Wall {};

class Hero {};

class Monster {};

// TODO: diagram with links to code snippets
