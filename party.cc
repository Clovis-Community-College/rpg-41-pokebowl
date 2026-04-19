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
        bank[i]->pos(history[i-1]);
    }
}
	
float Party::weather_scale(string weather) {
	// limitation: cnanot to per-actor scaling yet
	// but if have time - which means never
	if (weather == "Clear") return 1;// clear
	else if (weather == "Rain") return 0.9; // rain slippery
	else if (weather == "Fog") return 0.85; // fog blinds vision
	else if (weather == "Windy") return 1.1; // windy more "smooth-moving"
	else return 1; //clear as a fallback
}

void Party::kill(Actor* actor) {
	// container of the dead's assests
	IOrphan orphaned_inv{};
	int coins{};

	// item + coin transfer to a temp spot
	if (auto items = actor->items; items.has_value()) {
		orphaned_inv = items.value().drop_all(); // item transfer
		coins = items.value().get_coins(); //coin transfer
	}

	// coords transfer
	XY xy = actor->pos();

	// delete actor from cll -> bank -> itself(?)
	turn_order.list_delete(actor);
	std::erase_if(bank, [actor](Actor* a){ return a == actor; });
	delete actor;

	// make drop corresponding to actor
	Drop* drop = new Drop(xy, orphaned_inv, coins);
	bank.push_back(drop);
	add_member(drop);
}

void Party::inator() {
	// diffrent funciton for different stages?
	// be damned
	// im gassed

	// for god sake dont pass null in here
	auto interactable = [&](const Actor* actor){
		return	(actor->type() == "monster") ||
			(actor->type() == "hero") ||
			(actor->type() == "merchant") ||
			(actor->type() == "drop");
	};

	
	auto rankable = [&](const Actor* actor){
		return	(actor->type() == "monster") ||
			(actor->type() == "hero");
	};

	// create EVERYTHING then kill it outside init step

	vector<Actor*> bank_rankable;

	// A - Initialization != cstor!!!!!!!
	// A1 - first fill-in loop
	for (auto actor : bank) {
		// dont bother with null
		if (!actor) continue;

		// A1.a - speed() fill
		if (rankable(actor)) {
			bank_rankable.push_back(actor);
			
		}

		
	}

	// A2 - call any range-based function here

	// get the turn list
	auto cll_ptr = HasInitiative::begin_combat(bank_rankable);
	if (cll_ptr) turn_order = std::move(*cll_ptr);
}

void Party::you_spin_me_round() {
	// B - you spin round and round like a record
	while (!side_dead("monster") && !side_dead("hero")) {
		// trash bin code, MUST rewrite
		// section:

		// get current actor (list auto-advance)
		auto actor_pair = turn_order.current();
		if (actor_pair.second) { /* cycle lapsed */ }
		Actor* actor = actor_pair.first;

		// find first living opponent
		auto it = std::find_if(bank.begin(), bank.end(), [&actor](Actor* opponent){
			bool both_alive = !actor->is_dead() && !opponent->is_dead();
			bool monster_hits_hero = (actor->type() == "monster") && (opponent->type() == "hero");
			bool hero_hits_monster = (actor->type() == "hero")    && (opponent->type() == "monster");
			return both_alive && (monster_hits_hero || hero_hits_monster);
		});

		if (it == bank.end()) continue;
		Actor* opponent = *it;
	
	}
}
