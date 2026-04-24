#ifndef HITLIST_H
#define HITLIST_H

#include <deque>

class Actor;

struct Who {
	Actor* executor;
	Actor* victim;
	short dmg;
};

class Hitlist {
	using Bank = std::deque<Who>;
	Bank _zyklon{};
public:
	void shove(Actor* executor, Actor* victim, short dmg, string victim_type, string victim_name, string& last_action) {
		last_action += "\n\t\t\t";
		last_action += (victim_type == "monster") ? " - DEFEATED!" : "";
		last_action += (victim_type == "hero") ? (victim_name + " is down...") : "";	
		last_action += "\n\n";
		Who who = {executor, victim, dmg};
		_zyklon.push_back(who);
	}

	bool none() const {
		return _zyklon.empty();
	}

	Who pop_top() {
		if (_zyklon.empty()) return {nullptr, nullptr, 0};
		Who front = {_zyklon.front().executor, _zyklon.front().victim, _zyklon.front().dmg};
		_zyklon.pop_front();
		// hitlist.last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() +  " dealt " + std::to_string(dmg) + " dmg to " + opponent->name();
		return front;
	}
};

#endif
