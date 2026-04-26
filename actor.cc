#include "actor.h"

// Traits
Traits::Traits(AttackHP _do, InversedDefenseScale _hs, Speed _ss, HP _hpm,
			   WeatherScale _wsd)
	: hp_max(_hpm), attack_damage(_do), hurt_scale(_hs), starting_speed(_ss),
	  weather_scale_damage(_wsd) {}

// Actor
Actor::Actor(string init_name, XY init_xy, HP init_hp, Traits init_traits)
	: _name(init_name), _pos(init_xy), _hp(init_hp), _traits(init_traits),
	  items(nullopt) {
	items.emplace();
}

Actor::Actor(const Actor &actor) : _traits(actor._traits) {
	_name = actor._name;
	_pos = actor._pos;
	_hp = actor._hp;
	//	_hp_max = actor._hp_max;
	//	_damage_output = actor._damage_output;
	//	_starting_speed = actor._starting_speed;
	//	_damage_scale = actor._damage_scale;
}

Actor::~Actor() {}

Actor &Actor::operator=(const Actor &actor) {
	_name = actor._name;
	_pos = actor._pos;
	_hp = actor._hp;
	_traits = actor._traits;
	return *this;
}

Actor::Actor(Actor &&actor) = default;

Actor &Actor::operator=(Actor &&actor) = default;

string Actor::name() const { return _name; }
XY Actor::pos() const { return _pos; }
HP Actor::hp() const { return _hp; }
decltype(Actor::_traits.starting_speed) Actor::starting_speed() const {
	return _traits.starting_speed;
}
decltype(Actor::_traits.attack_damage) Actor::attack_damage() const {
	return _traits.attack_damage;
}
decltype(Actor::_traits.hp_max) Actor::hp_max() const {
	return _traits.hp_max;
}
// decltype(Actor::items) Actor::items() const { return items; }
decltype(Actor::_traits.weather_scale_damage)
Actor::weather_scale_damage() const {
	return _traits.weather_scale_damage;
}
decltype(Actor::_traits.hurt_scale) Actor::hurt_scale() const {
	return _traits.hurt_scale;
}

bool Actor::is_dead() const { return (_hp <= 0); }

bool Actor::is_self_with(const Actor *other) const { return this == other; }

bool Actor::share_type_with(const Actor *other) const {
	return type() == other->type();
}

void Actor::name(string _name_) { _name = _name_; }
void Actor::pos(XY _pos_) { _pos = _pos_; }

void Actor::weather_scale_damage(WeatherScale wsd) {
	_traits.weather_scale_damage = wsd;
}

void Actor::attack_damage(decltype(Actor::_traits.attack_damage) ad) {
	_traits.attack_damage = ad;
}

void Actor::hurt_scale(decltype(Actor::_traits.hurt_scale) hs) {
	_traits.hurt_scale = hs;
}

void Actor::hp(HP hp_delta, float external_scale,
				   const function<float(float, float)> &op) {
	if (hp_delta < 0) {
		std::cerr << "hp_delta less than 0";
		exit(EXIT_FAILURE);
	}
	float combined_hp_delta = hp_delta * external_scale * _traits.hurt_scale;
	float new_hp = op((float)hp(), combined_hp_delta);

	[&](HP _hp_) {
		if (_hp_ > _traits.hp_max)
			_hp = _traits.hp_max; // cap HP

		// -1 acts as a DEAD status code. potentially buggy.
		// can also use 0 for DEAD signal, if death procressing requires 2
		// frames
		else if (_hp_ < 0)
			_hp = -1;

		else
			_hp = _hp_;
	}(new_hp); // type narrowing intended
	// bcs in here, the needed rounding op is (arbitrarily chosen to be)
	// std::floor()
}

void Actor::take_damage(HP hp_delta, float external_damage_scale = 1) {
	auto op = [](float a, float b) -> float { return a - b; };
	hp(hp_delta, external_damage_scale, op);
}

void Actor::cure_damage(HP hp_delta, float external_heal_scale = 1) {
	auto op = [](float a, float b) -> float { return a + b; };
	hp(hp_delta, external_heal_scale, op);
}

void Actor::move(Direction d) {}

HP Actor::attack(Actor *opponent) {
	if (!opponent)
		return 0; // Null check. Just in case.

	HP delta = opponent->hp();

	// Condition check
	bool gtg_general =
		_good_to_attack(opponent); // GENERAL conditions for attack
	bool gtg_specific = _subclass_good_to_attack(
		opponent); // SUBCLASS-SPECIFIC conditions for attack
	if (!gtg_general || !gtg_specific)
		return 0;

	// now opponent ALWAYS lives and not itself
	_attack(opponent);

	// plug-in for custom attack behavior
	_post_attack(opponent);

	delta -= opponent->hp();
	delta -= opponent->hp() == -1;

	return delta;
}

bool Actor::_good_to_attack(Actor *opponent) const {
	// Cant attack oneself
	if (is_self_with(opponent))
		return false;

	// Cant attack a corpse
	if (opponent->is_dead())
		return false;

	// if opponent die, we leave its corpse there, no deletion
	// we can revive it like a zombie????????? hypothetical
	// if so move into subclass version
	return true;
}

bool Actor::_subclass_good_to_attack(Actor *opponent) const {
	// default behavior is to let all attack thru
	// (after general check)
	return true;
}

float Actor::_custom_scale() const { return 1.0;}

void Actor::_attack(Actor *opponent) {
	HP hp_delta = attack_damage();
	if (items.has_value()) {
		std::vector<Item> equipped;
		items->get_all_items(equipped);
		for (auto &item : equipped) {
			hp_delta += item.get_damage();
		}
	}
	float external_damage_scale = weather_scale_damage() * _custom_scale();
	opponent->take_damage(hp_delta, external_damage_scale);
}

void Actor::_post_attack(Actor *opponent) {
	// for subclasses
	// defaults to nothing
}

// Wall
Wall::Wall(XY xy) : Actor("wall", xy, HP_MAX, Traits(0, 0, 0, HP_MAX, 1)) {}

ActorType Wall::type() const { return "wall"; }

void Wall::_attack(Actor *opponent) {
	// no attack is possible if wall attacks sth
}

// Merchant

void Merchant::_attack(Actor *opponent) {
	// no attack is possible if wall attacks sth
	// TODO: open merchant shop and shit
	if (!items) // ifno inv then make one
		items.emplace();

	items->clear();				 // clears last stock
	items->gen_rand_loot(6, 12); // generates new stock every interactiom

	if (!opponent->items) // if actor doesnt have inventory then create theirs
		opponent->items.emplace();
}

ActorType Merchant::type() const { return "merchant"; }

// Drop
Drop::Drop(XY xy, IOrphan io, int coins) : Wall(xy), orphaned_items(io) {
	items.value().add_pokecoins(coins);
}

void Drop::_attack(Actor *opponent) {
	// no attack is possible if wall attacks sth
	// TODO: wait for Joe's pickup func
}

ActorType Drop::type() const { return "drop"; }

// NonWall

// template method not useless
// prevention - in case actor subgroup can be refactored
void NonWall::special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) { 
	subclass_special(bank, hitlist, exclude, last_action);
	int random = rand() % 10; // super rare
	
	if (random) return;
	
	random = rand() % 6;
	last_action += "\t{{ S**TTY EFFECTS }} " + this->name() + " ";
	switch (random) {
		case 0:
		    last_action += "saw Mencarelli worshipping Tralalelo Tralala, suffered immediate pump failure, and unalived itself.";
		    break;
		case 1:
		    last_action += "witnessed Ting Tung Tung Sahur's poster inside Guzman's bedroom, heart flatlined, and disconnected.";
		    break;
		case 2:
		    last_action += "checked the 2026 inflation rates, had a stroke and die.";
		    break;
		case 3:
		    last_action += "saw Claude Mythos start a family with 20 Olympic pools, then decided to logged off.";
		    break;
		case 4:
		    last_action += "saw the electricity bill for your 5090 Ti, bought 5 more for you, then joined the void.";
		    break;
		case 5:
		    last_action += "watched Kerney rambling in-class with Minecraft parkour overlay, brain short-circuited, and alt-f4'd.";
		    break;
		default:
		   last_action += "unalived itself. (also hi Rosas, i see u have /homework/ on VSCode!)";
		    break;
	}

	last_action += "\n";

	hitlist.shove(this, this, hp(), this->type(), this->name(), last_action);
}

bool NonWall::_subclass_good_to_attack(Actor *opponent) const {
	// no attack of same type - can be overriden
	bool two_actors_different_kind = (type() != opponent->type());
	return two_actors_different_kind;
}

// Hero
ActorType Hero::type() const { return "hero"; }

void Hero::move(Direction d) {
	XY p = pos();
	switch (d) {
	case UP:
		p.y--;
		break;
	case DOWN:
		p.y++;
		break;
	case LEFT:
		p.x--;
		break;
	case RIGHT:
		p.x++;
		break;
	default:
		break;
	}
	pos(p);
}

// H subs
Aleph::Aleph(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 750, Traits(80, 0.75, 1, 750, 1)) {
	// Snorlax-like
	// very high HP, mid attk, slightly worse defense BUT very slow speed, go
	// last
}

Bet::Bet(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 530, Traits(200, 1.75, 8, 500, 1)) {
	// glass cannon
}

Gimel::Gimel(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 400, Traits(60, 1, 19, 400, 1)) {
	// speedster - but "speed"-first not speed-first
}

Dalet::Dalet(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 800, Traits(20, 0.75, 10, 800, 1)) {
	// tbd: healer
}

He::He(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 300, Traits(100, 1, 13, 300, 1)) {
	// eevee(TM), the most "generic" Poke ever
}

Vav::Vav(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 250, Traits(170, 1.3, 12, 250, 1)) {
	// berserker
	// with great attack comes great fragility
}

Zayin::Zayin(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 1000, Traits(20, 0.4, 3, 1000, 1)) {
	// Shuckle
	// movable wall basically
}

Chet::Chet(string _name_, XY _pos_)
	: Hero(_name_, _pos_, 250, Traits(180, 1.2, 16, 250, 1)) {
	// Assasin
}

void Aleph::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	for (Actor *opponent : bank) {
		// workin with ptrs here
		if (!opponent || opponent == exclude)
			continue;
		if (opponent->type() != "monster")
			continue;

		// do damage
		// 1 - health delta
		HP dmg = this->attack(opponent);
		last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " dealt " + std::to_string(dmg) + " dmg pts to " + opponent->name();

		// 3 - dead or living?
		if (opponent->is_dead())
			hitlist.shove(this, opponent, dmg, opponent->type(), opponent->name(), last_action);
		else
			last_action += ". \n\t\t\t\t" + opponent->name() + " has " + std::to_string(opponent->hp()) + " HP left.\n\n";

	}
}

void Bet::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	if (_custom_scale() > 1.5) {
		const string scale = std::format("${:.2f}", _custom_scale());
		last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " just BOOSTED atck pwr by " + scale + "!\n\t\t\t\t";
	}
}

float Bet::_custom_scale() const {
	return log(hp_max()/(hp()+0.5))+1.5;
}

void Gimel::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// find min ally to heal
	auto it_ = std::find_if(bank.begin(), bank.end(), [](const Actor* a){ 
		if (!a) return false;
		else return (!a->is_dead() && a->type() == "hero"); 
	});

	auto it = std::min_element(it_, bank.end(), [exclude](const Actor* a, const Actor* b){
                // workin with ptrs here
                if (!a || !b) return false;
                bool b0 = a != exclude && b != exclude;
                bool b1 = (a->type() == "hero") && (b->type() == "hero");
                bool b2 = (a->hp()) < (b->hp());
		bool b3 = !a->is_dead() && !b->is_dead();
		return b0 && b1 && b2 && b3;
        });

	if (it == bank.end()) return;

	// do damage
	// 1 - health delta
	float heal_scale = log(20.0 / (((float)(*it)->hp() / (float)(*it)->hp_max()) + 2) - 6.25);
	HP dmg = round(std::clamp(heal_scale, 0.0f, 1.0f) * (*it)->hp_max());

	if (dmg <= 0) return;
	(*it)->cure_damage(dmg);
	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " boosted " +
				   std::to_string(dmg) + " HP to " + (*it)->name();

	last_action += ". \n\t\t\t\t" + (*it)->name() + " now has " +
				   std::to_string((*it)->hp()) + " HP.\n";

}

bool Dalet::_subclass_good_to_attack(Actor *opponent) const {
	// no attack of same type
	bool two_actors_different_kind = (type() != opponent->type());
	bool is_hero = opponent->type() == "hero";
	return two_actors_different_kind ||
		   is_hero; // logically always true, but semantically makes sense
}

void Dalet::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	HP cure_hp = round(attack_damage() * hurt_scale());
	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " stole " +
				   std::to_string(cure_hp) + " HP from a mosnter to itself!";

	last_action += ". \n\t\t\t\t" + this->name() + " now has " +
				   std::to_string(this->hp()) + " HP.\n";
}

void Dalet::_post_attack(Actor* opponent) {
	// lifesteal. ignore wetaher condition.
	HP cure_hp = round(attack_damage() * hurt_scale());
	cure_damage(cure_hp);
}

void He::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// ressurect
	// find ded ally
	auto it = std::find_if(bank.begin(), bank.end(), [](const Actor* a){ 
		if (!a) return false;
		else return (a->is_dead() && a->type() == "hero"); 
	});

	if (it == bank.end()) return;

	// do damage
	// 1 - health delta
	HP dmg = round(0.25 * (float)(*it)->hp_max());

	if (dmg <= 0) return;
	(*it)->cure_damage(dmg);
	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " revived " + (*it)->name();

	last_action += " \n\t\t\t\t" + (*it)->name() + " now has " +
				   std::to_string((*it)->hp()) + " HP.\n";

}

void Vav::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// find max opponent to double_punch
	auto it_ = std::find_if(bank.begin(), bank.end(), [](const Actor* a){ 
		if (!a) return false;
		else return (!a->is_dead() && a->type() == "monster"); 
	});

	auto it = std::max_element(it_, bank.end(), [exclude](const Actor* a, const Actor* b){
                // workin with ptrs here
                if (!a || !b) return false;
          //      bool b0 = a != exclude && b != exclude;
                bool b1 = (a->type() == "monster") && (b->type() == "monster");
                bool b2 = (a->hp()) < (b->hp());
		bool b3 = !a->is_dead() && !b->is_dead();
		return b1 && b2 && b3;
        });

	if (it == bank.end()) return;

	Actor* opponent = *it;

	// do damage
	// 1 - health delta
	HP dmg = this->attack(opponent);
	dmg += this->attack(opponent); // double attack
	last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " dealt double attack! Dealt " + std::to_string(dmg) + " dmg pts to " + opponent->name() + ".\n\t\t\t\t";

	// 3 - dead or living?
	if (opponent->is_dead())
		hitlist.shove(this, opponent, dmg, opponent->type(), opponent->name(), last_action);
	else
		last_action += opponent->name() + " has " + std::to_string(opponent->hp()) + " HP left.\n\n";

}

void Zayin::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// debuff enemy 12.5%
	// find a RANDOM enemy to debuff
	auto it = std::find_if(bank.begin(), bank.end(), [](const Actor* a){ 
		int random = rand() % 8;
		if (!a) return false;
		else return (!a->is_dead() && a->type() == "monster" && random == 0); 
	});

	if (it == bank.end()) return;

	Actor* opponent = *it;

	int random = rand() % 2;

	// decrease defense by 10%, OR decrease attk damage by 15%
	if (random) opponent->hurt_scale(opponent->hurt_scale() * 1.1);
	else opponent->attack_damage(opponent->attack_damage() * 0.85f);

	last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + "has debuffed " + opponent->name() + "! " + opponent->name() + "'s";

	string scale;

	if (random) {
		scale = std::format("${:.2f}", opponent->hurt_scale());
		last_action += "defense is only " + scale + "x as strong!";
	}
	else
		last_action += "attack is 15% weaker than before!";

	last_action += "\n\t\t\t\t";
}

void Chet::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// find opponent in bank
	auto it_ = std::find_if(bank.begin(), bank.end(), [exclude](const Actor* a){ 
		if (!a) return false;
		else return (!a->is_dead() && a == exclude); 
	});

	// find NEXT opponent
	auto it = std::find_if(it_, bank.end(), [exclude](const Actor* a){
                // workin with ptrs here
                if (!a) return false;
                bool b0 = a != exclude;
                bool b1 = (a->type() == "monster");
                bool b2 = true;
		bool b3 = !a->is_dead();
		return b0 && b1 && b2 && b3;
        });

	if (it == bank.end()) return;

	// do damage
	HP dmg = round(_custom_scale() * attack_damage());

	if (dmg <= 0) return;

	// this wont take in EXTERNAL scale dmg bcs ehhhhhh
	(*it)->take_damage(dmg);

	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " spreaded 50% damage to " + (*it)->name() + "! " +
				   std::to_string(dmg) + " HP to " + (*it)->name();

	last_action += ". \n\t\t\t\t" + (*it)->name() + " now has " +
				   std::to_string((*it)->hp()) + " HP.\n";
}

float Chet::_custom_scale() const {
	return 0.5;
}

// Monster
ActorType Monster::type() const { return "monster"; }

Monster::Monster(string init_name, XY init_xy, HP init_hp, Traits init_traits)
	: NonWall(init_name, init_xy, init_hp, init_traits) {
	// emplace() already in ctor, so no nullopt check
	items.value().gen_rand_loot(2, 5);
	if (is_boss())
		items.value().insert(Item("Boss Key", "Key", 0, 0, 0, true));
}

bool Monster::is_boss() const { return false; }

// M sub
bool Foxtrot::is_boss() const { return true; }

Alpha::Alpha(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 100, Traits(40, 1.25, 5, 100, 1)) {
	// Rattata
	// alpha version monster, not alpha monster
}

Bravo::Bravo(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 60, Traits(15, 1, 15, 60, 1)) {
	// Zubat
	// weaker than rattata. but SHOULD BE more numerous?
}

Charlie::Charlie(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 425, Traits(120, 0.9, 6, 425, 1)) {
	// Machoke
	// ADD SPECIAL KEY TYPE TO INVENTORY
}

Delta::Delta(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 200, Traits(160, 1.4, 12, 200, 1)) {
	// Gengar, another glass cannon
}

Echo::Echo(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 300, Traits(100, 1, 10, 300, 1)) {
	// Ditto
}

Foxtrot::Foxtrot(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 1500, Traits(150, 0.5, 14, 2500, 1)) {
	// BOSS - mewtwo
}

Golf::Golf(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 400, Traits(20, 0.7, 8, 400, 1)) {
	// Blissey, healer for monsters
}

Hotel::Hotel(string _name_, XY _pos_)
	: Monster(_name_, _pos_, 220, Traits(140, 1.1, 19, 220, 1)) {
	// Persian
}

void Alpha::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// ape together strong
	// count mobstera
	char count = std::count_if(bank.begin(), bank.end(), [](const Actor* a){ 
		if (!a) return false;
		else return (!a->is_dead() && a->type() == "monster"); 
	});

	// boost damge. due to logic, will apply to NEXT turn.
	// only activates when its ALPHA turn.
	decltype(attack_damage()) oad = attack_damage();

	// only boost when 3+ monsters
	if (count < 3) {
		attack_damage((float)oad / 3.0f);
		return;
	}

	attack_damage((float)oad * 3.0f);

	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " triple-buffed attack! ";
}

void Bravo::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	// find min opponent to heal
	auto it_ = std::find_if(bank.begin(), bank.end(), [](const Actor* a){ 
		if (!a) return false;
		else return (!a->is_dead() && a->type() == "monster"); 
	});

	auto it = std::min_element(it_, bank.end(), [exclude](const Actor* a, const Actor* b){
                // workin with ptrs here
                if (!a || !b) return false;
                bool b0 = a != exclude && b != exclude;
                bool b1 = (a->type() == "monster") && (b->type() == "monster");
                bool b2 = (a->hp()) < (b->hp());
		bool b3 = !a->is_dead() && !b->is_dead();
		return b0 && b1 && b2 && b3;
        });

	if (it == bank.end()) return;

	// do damage
	// 1 - health delta
	float heal_scale = log(20.0 / (((float)(*it)->hp() / (float)(*it)->hp_max()) + 2) - 6.25);
	HP dmg = round(std::clamp(heal_scale, 0.0f, 1.0f) * (*it)->hp_max());

	if (dmg <= 0) return;
	(*it)->cure_damage(dmg);
	last_action += "\t{{ SPECIAL EFFECTS }} " + this->name() + " boosted " +
				   std::to_string(dmg) + " HP to " + (*it)->name();

	last_action += ". \n\t\t\t\t" + (*it)->name() + " now has " +
				   std::to_string((*it)->hp()) + " HP.\n";

}

void Charlie::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	int random = rand() % 16; // super rare
	if (random) return;
	hurt_scale(1); // break armor
	last_action += "\n\t{{ S**TTY EFFECTS }} " + this->name() + " saw Rosas' VSCode \"howework folder\" and lost armor!\n";
}

void Delta::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	int random = rand() % 125;
	if (!random) return;
	cure_damage(random);
	last_action += "\n\t{{ SPECIAL EFFECTS }} " + this->name() + " healed itself by " + + " HP!\n";
}

void Echo::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	if (!exclude) return;
	if (exclude->is_dead()) return;

	Actor*& opponent = exclude;

	// do damage
	// 1 - health delta
	HP dmg = this->attack(opponent);
	dmg += this->attack(opponent); // double attack
	last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " dealt a double attack! Dealt " + std::to_string(dmg) + " dmg pts to " + opponent->name();

	// 3 - dead or living?
	if (opponent->is_dead())
		hitlist.shove(this, opponent, dmg, opponent->type(), opponent->name(), last_action);
	else
		last_action += ". \n\t\t\t\t" + opponent->name() + " has " + std::to_string(opponent->hp()) + " HP left.\n\n";

}

void Foxtrot::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	for (Actor *opponent : bank) {
		// workin with ptrs here
		if (!opponent || opponent == exclude)
			continue;
		if (opponent->type() != "hero")
			continue;

		// do damage
		// 1 - health delta
		HP dmg = this->attack(opponent);
		last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " dealt " + std::to_string(dmg) + " dmg pts to " + opponent->name();

		// 3 - dead or living?
		if (opponent->is_dead())
			hitlist.shove(this, opponent, dmg, opponent->type(), opponent->name(), last_action);
		else
			last_action += ". \n\t\t\t\t" + opponent->name() + " has " +
						   std::to_string(opponent->hp()) + " HP left.\n\n";
	}
}

void Golf::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {}

void Hotel::subclass_special(Bank& bank, Hitlist& hitlist, Actor *exclude, string& last_action) {
	if (!exclude) return;
	if (exclude->is_dead() || exclude->hp() >= (0.4 * exclude->hp_max())) return;

	Actor*& opponent = exclude;

	// do damage
	// 1 - health delta
	decltype(attack_damage()) oad = attack_damage();
	attack_damage(oad * 0.3);

	HP dmg = this->attack(opponent);
	last_action += "\n\t{{SPECIAL EFFECT}} " + this->name() + " bullied " + opponent->name() +  "! Dealt extra " + std::to_string(dmg) + " dmg pts to " + opponent->name();
	attack_damage(oad);

	// 3 - dead or living?
	if (opponent->is_dead())
		hitlist.shove(this, opponent, dmg, opponent->type(), opponent->name(), last_action);
	else
		last_action += ". \n\t\t\t\t" + opponent->name() + " has " + std::to_string(opponent->hp()) + " HP left.\n\n";
}
