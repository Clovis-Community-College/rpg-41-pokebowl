#pragma once
#include "/public/read.h"
#include <string>
using namespace std;

class Item{
	string name;
	string itemtype;
	int cost;
	int heal;
	int damage;
	bool special;

	public:
	Item();
	Item(const string& newname, const string& newtype,int newcost,int newheal,int newdamage, bool newspecial);

	string get_name() const;
	string get_type() const;
	int get_cost() const;
	int get_heal() const;
	int get_damage() const;
	bool is_special() const;


	void set_name(const string& newname);
	void set_type(const string& newtype);
	void set_cost(int newcost);
	void set_heal(int newheal);
	void set_damage(int newdamage);
	void set_special(bool newspecial);

};




