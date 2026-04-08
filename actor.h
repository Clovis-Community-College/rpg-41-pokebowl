#include <string>

using std::string, std::cout, std::cin;
using HP = int;

// Map could have positive coords or negative-positive coords
// int for flexibility
struct XY {
	int x, y;
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
	constexpr int MAX_HP = INT_MAX;

public:
	// Move behaviour. TBI by subclasses.
	// Actor should only move on integer-based steps
	// (i.e., actors on a chessboard).
	virtual move(Direction d) = 0;

	// Do damage to another Actor.
	// Defaults to no damage.
	virtual take_damage(HP delta);
};

class Monster {};

class Hero {};

