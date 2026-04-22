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

/*
bool Party::side_dead(ActorType type) const {
	for (auto actor : bank) {
		if (actor && actor->type() == type && !actor->is_dead()) return false;
	}
	return true;
}
*/

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
	// assumes bank r non-null and ALL LIVING. 
	// HAVE TO PROCESS TO MATCH INVARIANT B4 EXIT
	if (status == hero_wins || status == monster_wins) return;

	status = ongoing;

	auto actor_pair = turn_order.current();
	Actor* actor = actor_pair.first;

	// tf is this
	// "send" a msg when avtor is down.
	// TODO: mgiht be move downward idk
/*	if (!actor || actor->is_dead()) {
		if (actor && actor->is_dead()) {
			last_action = actor->name() + " is down...";
		}
		if (side_dead(ActorType("monster"))) status = hero_wins;
		else if (side_dead(ActorType("hero"))) status = monster_wins;
		return;
	}
*/
	auto fightable = [&actor](Actor* opponent){
		if (!opponent) return false;
		bool both_alive = !actor->is_dead() && !opponent->is_dead();
		bool monster_hits_hero = (actor->type() == "monster") && (opponent->type() == "hero");
		bool hero_hits_monster = (actor->type() == "hero")    && (opponent->type() == "monster");
		return both_alive && (monster_hits_hero || hero_hits_monster);
	};

	// get opponent stage
	auto it = std::find_if(bank.begin(), bank.end(), fightable);
	
	// unnecessary, checked above.
	// TODO: handle it.end() code path

/*	if (it == bank.end()) {
		if (side_dead(ActorType("monster"))) status = hero_wins;
		else if (side_dead(ActorType("hero"))) status = monster_wins;
		return;
	}
*/

	Actor* opponent = *it;

	int hp_before = opponent->hp();
	actor->attack(opponent);
	int hp_after = opponent->hp();
	int dmg = hp_before - hp_after;

	if (opponent->is_dead()) {
		last_action = actor->name() + " dealt " + std::to_string(dmg) + " dmg to " + opponent->name() + " - DEFEATED!";
	} else {
		last_action = actor->name() + " dealt " + std::to_string(dmg) + " dmg to " + opponent->name() + " (" + std::to_string(hp_after) + " HP left)";
	}

	if (side_dead(ActorType("monster"))) status = hero_wins;
	else if (side_dead(ActorType("hero"))) status = monster_wins;
}
