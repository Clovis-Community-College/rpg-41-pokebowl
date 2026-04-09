#include "actor.h"
#include <cstdint>
#include "llbridges.h"

void HasInitiative::add_speed() {
	_speed += ((rand() % 20) + 1);	
} 

void HasInitiative::edit_speed(int8_t val) {
	_speed = val;
}

void HasInitiative::delete_speed() {
	_speed = 1;
}

decltype(HasInitiative::_speed) HasInitiative::speed() {
	return _speed;
};
