#include "actor.h"
#include <cstdint>
#include "llbridges.h"

using namespace std;

void HasInitiative::check_speed() {
	cerr << "_speed value error\n";
	exit(EXIT_FAILURE);
}

void HasInitiative::add_speed() {
	_speed += ((rand() % 20) + 1);	
	check_speed();
} 

void HasInitiative::edit_speed(int8_t val) {
	_speed = val;
	check_speed();
}

void HasInitiative::delete_speed() {
	_speed = 1;
}

decltype(HasInitiative::_speed) HasInitiative::speed() {
	return _speed;
};
