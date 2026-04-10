#include "actor.h"
#include <ctime> //to seed d20 rolls in speed(). see llbridges.cc

int main() { 
	srand(time({}));
	Wall w({0,0});
	Aleph h("hero", {0,1});
	Alpha m("monster", {0,2});
	Hero h1("",{0,0},22);
}
