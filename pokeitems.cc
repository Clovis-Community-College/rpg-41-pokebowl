#include "pokeitems.h"
#include "/public/read.h"
#include <string>
using namespace std;

Item::Item():name(""),itemtype(""),cost(0),heal(0),damage(0),special(false){}

Item::Item(const string& newname, const string& newtype,int newcost,int newheal,int newdamage, bool newspecial):name(newname), itemtype(newtype), cost(newcost),heal(newheal),damage(newdamage),special(newspecial){}

string Item::get_name() const{
	return name;
}

string Item::get_type() const{
	return itemtype;
}

int Item::get_cost() const{
	return cost;
}

int Item::get_heal() const{
	return heal;
}

int Item::get_damage() const{
	return damage;
}

bool Item::is_special() const{
	return special;
}


void Item::set_name(const string& newname){
	name=newname;
}

void Item::set_type(const string& newtype){
	itemtype=newtype;
}

void Item::set_cost(int newcost){
	cost=newcost;
}

void Item::set_heal(int newheal){
	heal=newheal;
}

void Item::set_damage(int newdamage){
	damage=newdamage;
}

void Item::set_special(bool newspecial){
	special=newspecial;
}







