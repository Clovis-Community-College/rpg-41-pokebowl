#include "party.h"

void Party::add_member(Actor* actor) {
    bank.push_back(actor);
}

void Party::init_history(XY initial_pos) {
    history.clear();
    for (int i = 0; i < 6; i++) {
//	if (bank[i]->type() != "hero") continue;
        history.push_back(initial_pos);
    }
}

void Party::record_move(XY old_pos) {
    history.push_front(old_pos);
    if (history.size() > 6) {
        history.pop_back();
    }
    for (size_t i = 1; i < bank.size(); ++i) {
	if (bank[i]->type() != "hero") continue;
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

// INDIVIDUAL dead only
void Party::post_mortem(Actor* actor, bool gen_drop = true) {
	// just-in-case™
	if (!actor || !actor->is_dead()) return;

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

	if (!gen_drop) {
		if (actor->type() == "drop" || actor->type() == "merchant" || actor->type() == "wall") delete actor;
		return;
	}

	// make drop corresponding to actor
	Drop* drop = new Drop(xy, orphaned_inv, orphaned_coins);
	add_member(drop); // vector pushback != rendering !!!!!! latter UNimplemented?

	last_action += (actor->type() == "monster") ? " - DEFEATED!" : "";
	last_action += (actor->type() == "hero") ? ("\n" + actor->name() + " is down...") : "";

	dead_count++;
}

void Party::corpse_incinerator(bool forced = false) {
	if (dead_count < 2 && !forced) return;

	// CLL cleanup
	turn_order.reset_current_to_start();
	for (std::pair<Actor*, bool> actor_pair; (actor_pair = turn_order.current(), (actor_pair.first && actor_pair.second == false));) {
	        Actor* actor = actor_pair.first;
		if (!actor) continue;
		else if (actor->is_dead()) turn_order.list_delete(actor);
	}

	// vector clean-up
	std::erase_if(bank, [](Actor *actor){ 
		if (!actor) return false;
		return actor->is_dead();
	});

	dead_count = 0;
}

void Party::inator() {
	// diffrent funciton for different stages?
	// be damned
	// im gassed

	corpse_incinerator();
	status = init;
	cycles_left = 2;

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
	if (status != ongoing && status != init) return;
	status = (cycles_left && status != hero_wins && status != monster_wins) ? ongoing : cycle_ends;
	if (status == cycle_ends) return;

	corpse_incinerator();

	auto actor_pair = turn_order.current();
	if (actor_pair.second) cycles_left--;
	Actor* actor = actor_pair.first;

//	last_action += "\nACTOR: " + actor->name() + " - TYPE: " + actor->type() + " - CYCLE LEFT: " + std::to_string(cycles_left); 

	auto fightable = [&](Actor* opponent){
		if (!opponent) return false;
		bool both_alive = !actor->is_dead() && !opponent->is_dead();
		bool monster_hits_hero = (actor->type() == "monster") && (opponent->type() == "hero");
		bool hero_hits_monster = (actor->type() == "hero")    && (opponent->type() == "monster");
		bool fight = both_alive && (monster_hits_hero || hero_hits_monster);
//		string t = ((fight)?"OK":"STALL");
//		last_action += "\n[VEC] OPPONENT " + t + ": " + opponent->name() + " - TYPE: " + opponent->type() + " - cycles_left: " + std::to_string(cycles_left); 
		return fight;
	};

	// get opponent stage
	auto it = std::find_if(bank.begin(), bank.end(), fightable);
	
	if (it == bank.end()) {
		// tally side state. who wins?
		std::pair<Actor*, bool> actor_pair;
		turn_order.reset_current_to_start();
		while (!actor_pair.second) {
			actor_pair = turn_order.current();
			Actor* a = actor_pair.first;
			if (actor && actor->type() == "hero" && !actor->is_dead()) { status = hero_wins; break; }
			else if (actor && actor->type() == "monster" && !actor->is_dead()) { status = monster_wins; break; }
		} // if hit Merchant, Drop, or other types, keep moving with current()
		return;
	}

	Actor* opponent = *it;

	// do damage
	//1 - health delta
	HP dmg = actor->attack(opponent);
	last_action += "\n" + actor->name() + " dealt " + std::to_string(dmg) + " dmg to " + opponent->name();

	// 2 - dead or living?
	if (opponent->is_dead()) 
		post_mortem(opponent);
	else
		last_action += ". " + opponent->name() + " has " + std::to_string(opponent->hp()) + " HP left.";

//	if (side_dead(ActorType("monster"))) status = hero_wins;
//	else if (side_dead(ActorType("hero"))) status = monster_wins;
}
