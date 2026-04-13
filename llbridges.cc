#include "actor.h"
#include <cstdlib>
#include "llbridges.h"
#include "circularll.h"

using namespace std;

void HasInitiative::begin_combat(vector<Actor*> involved) {
	vector<Actor*> heroL;
	vector<Actor*> enemyL;
	for (auto x : involved) {
		speed(x);
		if (x->type() == "hero") {
			heroL.push_back(x);
		} else if (x->type() == "monster") {
			enemyL.push_back(x);
		}
		/*
		while (heroL.size() or enemyL.size()) {
			if (heroL.size()) {
				for (int c = 0; c < heroL.size(); c++) {
					for (int d = 0; d < heroL.size(); d++) {	
					}
				}
			}
			if (enemyL.size()) {
				
			}
		}*/
	}
	CLL linkeddList;
};

void HasInitiative::check_speed() {
	if (_speed < 1 || _speed > 60) {
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
