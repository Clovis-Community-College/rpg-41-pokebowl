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
	Node* rdelete(Node* temp, const string& name, bool& deleted);
	Node* rmin(Node* temp) const;

	public:
	Inventory();
	~Inventory();

	void insert(const Item& item);
	bool deletes(const string &name);
	bool drop(const string& name);
	bool contains(const string& name) const;
	int getItemCount(const string& name) const;
	void print() const;

	void addpokecoins(int amount);
	bool spendcoins(int amount);

	bool buy(const Item& item);
	bool sell(const string& name);

	int getsize() const;
	int getcoins() const;


};


