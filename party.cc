#include "party.h"

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

void Party::kill(Actor* actor, bool gen_drop = true) {
	// container of the dead's assests
	IOrphan orphaned_inv{};
	int orphaned_coins{};

	// item + coin transfer to a temp spot
	if (auto items = actor->items; items.has_value()) {
		orphaned_inv = items.value().drop_all(); // item transfer
		orphaned_coins = items.value().get_coins(); //coin transfer
	}

	// coords transfer
	XY xy = actor->pos();

	// delete actor from cll -> bank -> itself(?)
	turn_order.list_delete(actor);
	std::erase_if(bank, [actor](Actor* a){ return a == actor; });
	delete actor;

	if (!gen_drop) return;

	// make drop corresponding to actor
	Drop* drop = new Drop(xy, orphaned_inv, orphaned_coins);
	add_member(drop); // vector pushback != rendering !!!!!! latter UNimplemented?
}

void Party::inator() {
	// diffrent funciton for different stages?
	// be damned
	// im gassed

	status = init;

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

	vector<Actor*> bank_rankable;

	for (auto actor : bank) {
		// dont bother with null
		if (!actor) continue;

		if (rankable(actor))
			bank_rankable.push_back(actor);		
	}

	// get the turn list
	auto cll_ptr = HasInitiative::begin_combat(bank_rankable);
	if (cll_ptr) turn_order = std::move(*cll_ptr); // move, or point to the void
}

void Party::one_more_time() {
	// if wins lose no more turn
	if (status == hero_wins || status == monster_wins) return;

	status = ongoing;

	// get current actor (list auto-advance)
	auto actor_pair = turn_order.current();
	if (actor_pair.second) { /* cycle has lapsed */ }
	Actor* actor = actor_pair.first;

	// find first living opponent
	auto fightable = [&actor](Actor* opponent){
		bool both_alive = !actor->is_dead() && !opponent->is_dead();
		bool monster_hits_hero = (actor->type() == "monster") && (opponent->type() == "hero");
		bool hero_hits_monster = (actor->type() == "hero")    && (opponent->type() == "monster");
		return both_alive && (monster_hits_hero || hero_hits_monster);
	};

	auto it = std::find_if(bank.begin(), bank.end(), fightable);

	Actor* opponent = *it;

	// One-turn attack
	actor->attack(opponent);

	// opponent might die, who knows
	if (opponent->is_dead()) kill(opponent);

	// find second opponent to determine state
	// its o(N) anw
	auto it_next = std::find_if(it, bank.end(), fightable);
		
	// check wining cond
	if (it_next == bank.end()) {
		// Make sure there r decent 
		// None of that no-or-1-or-2 actors inside
		if (actor->type() == "hero") status = hero_wins;
		else if (actor->type() == "monster") status = monster_wins;
	}
}
