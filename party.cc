#include "party.h"

bool Party::side_dead(ActorType type) const {
	auto it = std::find_if(bank.begin(), bank.end(), [&](const Actor* a) -> bool {
		bool not_null = a;
		bool is_living = !a->is_dead();
		bool is_type = a->type() == type;
		return not_null && is_living && is_type;
	});

	bool no_remaining_type = (it == bank.end());
	return no_remaining_type;
}

float Party::weather_scale(string weather) const {
	if (weather == "Clear") return 1;
	else if (weather == "Rain") return 0.9;
	else if (weather == "Fog") return 0.85;
	else if (weather == "Windy") return 1.1;
	else return 1;
}

void Party::add_member(Actor* actor) {
    bank.push_back(actor);
}

void Party::init_history(XY initial_pos) {
    history.clear();
    for (int i = 0; i < 6; i++) {
        history.push_back(initial_pos);
    }
}

void Party::record_move(XY old_pos) {
    history.push_front(old_pos);
    if (history.size() > 6) {
        history.pop_back();
    }
    for (size_t i = 1; i < bank.size(); ++i) {
        bank[i]->set_pos(history[i-1]);
    }
}
