#include "actor.h"
#include <cstdint>
#include <cstdlib>
#include "llbridges.h"
#include "circularll.h"

using namespace std;

//int8_t HasInitiative::get_speed() { return _speed; }

unique_ptr<CLL> HasInitiative::begin_combat(vector<Actor*> involved) {
	// random speed fill stage
	for (auto actor : involved) { 
		// defensive, as input vector is already only has mobster and hero
		// but... Just-in-Case™
		if (actor->type() != "monster" && actor->type() != "hero") continue;
		auto h_actor = dynamic_cast<HasInitiative*>(actor);
		h_actor->speed(actor);
	}

	sort(involved.begin(), involved.end(), [](Actor* a, Actor* b){
			HasInitiative* nuA = dynamic_cast<HasInitiative*>(a);
			HasInitiative* nuB = dynamic_cast<HasInitiative*>(b);
			return nuA->_speed > nuB->_speed;
		});
	auto combatOrder = make_unique<CLL>();
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

int8_t HasInitiative::get_speed() {
	return _speed;
}
