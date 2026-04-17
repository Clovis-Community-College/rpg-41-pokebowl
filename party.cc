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

float Party::weather_scale(string weather) const {
	// limitation: cnanot to per-actor scaling yet
	// but if have time - which means never
	if (weather == "Clear") return 1;// clear
	else if (weather == "Rain") return 0.9; // rain slippery
	else if (weather == "Fog") return 0.85; // fog blinds vision
	else if (weather == "Windy") return 1.1; // windy more "smooth-moving"
	else return 1; //clear as a fallback
}
