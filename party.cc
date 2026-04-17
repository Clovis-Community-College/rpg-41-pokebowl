#include "party.h"

// the pseudocode on personal plan file
// handle gold pokecoin pasisng using insrt and add_pokecoins or blablabal
// so basically EXISTING FUNCTIONS.

bool Party::side_dead(ActorType type) const {
	auto it = std::find_if(bank.begin(), bank.end(), [&](const Actor* a) -> bool {
		// Check for the first living type...
		bool not_null = a;
		bool is_living = !a->is_dead();
		bool is_type = a->type() == type;
		return not_null && is_living && is_type;
	});

	// ...then inverts it.
	bool no_remaining_type = (it == bank.end());

	return no_remaining_type;
}
