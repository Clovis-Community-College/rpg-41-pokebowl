#include "pokeitems.h"
#include "/public/read.h"
#include <string>
using namespace std;

Item::Item():name(""),itemtype(""),cost(0),ballmultiplier(0),special(false){}
Item::Item(const string& newname, const string& newtype,int newcost,int new_ballmulitplier, bool newspecial):name(newname), itemtype(newtype), cost(newcost),ballmultiplier(new_ballmulitplier),special(newspecial){}

string Item::get_name() const{
	return name;
}

string Item::get_type() const{
	return itemtype;
}

int Item::get_cost() const{
	return cost;
}

int Item::get_ballmultiplier() const{
	return ballmultiplier;
}

bool Item::is_special() const{
	return special;
}






