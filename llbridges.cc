#include "actor.h"
#include <cstdlib>
#include "llbridges.h"
#include "circularll.h"

using namespace std;

//int8_t HasInitiative::get_speed() { return _speed; }

CLL* HasInitiative::begin_combat(vector<Actor*> involved) {
	sort(involved.begin(), involved.end(), [](Actor* a, Actor* b){return a->new_speed() < b->new_speed();});
	CLL* combatOrder = new CLL;
	for (int i = 0; i < involved.size(); i++) { //insert all vector elements into the CLL
		combatOrder->list_insert(involved.at(i));
	}

	return combatOrder;

};

void HasInitiative::check_speed() {
	if (_speed < 1 || _speed > 60) {
		cerr << "_speed value error\n";
		exit(EXIT_FAILURE);
	}
}

decltype(HasInitiative::_speed)  HasInitiative::speed( Actor* actor) {
	_speed = actor->starting_speed() + ((rand() % 20) + 1);
	check_speed();
	return _speed;
};

void HasInitiative::reset_speed(Actor* actor) {
	_speed = actor->starting_speed();
	check_speed();
}
