#include "inventory.h"
#include "/public/read.h"
using namespace std;

Inventory::Inventory():root(nullptr), pokecoins(0), size(0){}
Inventory::~Inventory(){
	extinguish(root);
}

void Inventory::extinguish(Node* temp){
	if(temp==nullptr)
		return;

	extinguish(temp->left);
	extinguish(temp->right);
	delete temp;
}

void Inventory::rprint(Node* temp) const{
	if (temp==nullptr)
		return;

	rprint(temp->left);
	cout<<temp->data.get_name()<<" x "<<temp->count<<endl;
	rprint(temp->right);
}


Inventory::Node* Inventory::rinsert(Node* temp, const Item& item){
	if (temp==nullptr){
		size++;
		return new Node(item);
	}

	if(item.get_name() < temp->data.get_name())
		temp->left = rinsert(temp->left,item);
	else if(item.get_name() > temp->data.get_name())
		temp->right = rinsert(temp->right,item);
	else {
		temp->count++;
	}
	return temp;
}
	
Inventory::Node* Inventory::
Node* Inventory::
Node* Inventory::
