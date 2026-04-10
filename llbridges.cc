#include "actor.h"
#include <cstdlib>
#include "llbridges.h"

using namespace std;

void HasInitiative::check_speed() {
	if (_speed < 1 || _speed > 40) {
		cerr << "_speed value error\n";
		exit(EXIT_FAILURE);
	}
}

decltype(HasInitiative::_speed) HasInitiative::speed( Actor* actor) {
	_speed = actor->starting_speed() + ((rand() % 20) + 1);
	check_speed();
	return _speed;
};

void HasInitiative::reset_speed(Actor* actor) {
	_speed = actor->starting_speed();
	check_speed();
}
