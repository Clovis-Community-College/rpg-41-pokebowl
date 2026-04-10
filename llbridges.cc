#include "actor.h"
#include "llbridges.h"

using namespace std;

decltype(HasInitiative::_speed) HasInitiative::speed( Actor* actor) {
	_speed = actor->start_speed() + ((rand() % 20) + 1);
	return _speed;
};

void HasInitiative::reset_speed(Actor* actor) {
	_speed = actor->start_speed();
}
