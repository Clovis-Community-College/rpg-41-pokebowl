#pragma once

#include "/public/read.h"
#include <string>
#include "pokeitems.h"
using namespace std;

class Inventory{
	struct Node{
		Item data;
		Node* left;
		Node* right;
		int count;

		Node(const Item& item): data(item),left(nullptr),right(nullptr),count(1){};
	};

	Node* root;
	int pokecoins;
	int size;

	void rprint(Node* temp) const;
	void extinguish(Node* temp);

	Node* rinsert(Node* temp, const Item& item);
	Node* rsearch(Node*temp, const string& name) const;

	public:
	Inventory();
	~Inventory();
	
	//some funcs will be bools to be used as checkers for other functions, will also perform act within function before returning true/false
	void insert(const Item& item);
	bool remove(const string &name);///special key items can be removed for quest reasons only
	bool drop(const string& name);///note that special key items can not be dropped
	bool contains(const string& name) const;
	int get_item_count(const string& name) const;
	void print() const;

	void add_pokecoins(int amount);
	bool spend_coins(int amount);

	bool buy(const Item& item);
	bool sell(const string& name);

	int get_size() const;
	int get_coins() const;


};


