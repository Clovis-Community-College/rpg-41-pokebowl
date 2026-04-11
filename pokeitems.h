#pragma once
#include "/public/read.h"
#include <string>
using namespace std;

class Item{
	string name;
	string itemtype;
	int cost;
	int ballmultiplier;
	bool special;

	public:
	Item();
	Item(const string& newname, const string& newtype,int newcost,int new_ballmulitplier, bool newspecial);

	string get_name() const;
	string get_type() const;
	int get_cost() const;
	int get_ballmultiplier() const;
	bool is_special() const;

};




