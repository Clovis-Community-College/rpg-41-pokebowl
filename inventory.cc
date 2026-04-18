#include "inventory.h"
#include "/public/read.h"
#include <ctime>
#include <cstdlib>



using namespace std;

//For random inventory generation
static bool range_initialized=false;
static void initizialize_randomgen(){
	if(!range_initialized){
		srand(time(0));
		range_initialized=true;
	}
}

//NEED TO FINISH RANDOM ITEMS TO BE SLECTED 0-9
static Item genrand_item(){
	int roll = rand()% 10;
	if (roll==0)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==1)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==2)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==3)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==4)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==5)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==6)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==7)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==8)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	if (roll==9)
		return Item("Item name tbd","item type tbd",0,0,0,false);
	// default return so compiler doesnt whine
	return Item("YSK Panera has fired all of its baking staff and replaced them with frozen product.", "If I go to a cookout and bite a hot dog and it's a carrot I'm fighting everybody", 0,0,0,false);
}

//WILL USE IN MONSTER CONSTRUCTORS EVENTUALLY
void Inventory::gen_rand_loot(int minitems,int maxitems){
	initizialize_randomgen();
	int amount=rand()%(maxitems-minitems+1)+minitems;

	for(int i=0;i<amount;i++)
		insert(genrand_item());
}

Inventory::Inventory() : root(nullptr), pokecoins(0), size(0) {}
Inventory::~Inventory() { extinguish(root); }

Inventory::Inventory(const Inventory& other){
	root=nullptr;
	size=0;
	pokecoins=other.pokecoins;
	vector<Item> copyitems;
	other.get_all_items(copyitems);

	for (const Item& item:copyitems)
		insert(item);
}

void Inventory::extinguish(Node *temp) {
  if (temp == nullptr)
    return;

  extinguish(temp->left);
  extinguish(temp->right);
  delete temp;
}
void Inventory::collect_items(Node* temp, vector<Item>& list) const{
	if(temp==nullptr)
		return;
	collect_items(temp->left,list);
	
	for(int i=0;i<temp->count;i++)
		list.push_back(temp->data);

	collect_items(temp->right,list);
}

void Inventory::get_all_items(vector<Item>& list) const{
	collect_items(root,list);
}

vector<Item> Inventory::drop_all(){
	vector<Item> out;
	get_all_items(out);
	clear();
	return out;
}


void Inventory::rprint(Node *temp) const {
  if (temp == nullptr)
    return;

  rprint(temp->left);
  cout << temp->data.get_name() << " x " << temp->count << endl;
  rprint(temp->right);
}

Inventory::Node *Inventory::rinsert(Node *temp, const Item &item) {
  if (temp == nullptr) {
    size++;
    return new Node(item);
  }

  if (item.get_name() < temp->data.get_name())
    temp->left = rinsert(temp->left, item);
  else if (item.get_name() > temp->data.get_name())
    temp->right = rinsert(temp->right, item);
  else {
    temp->count++;
  }
  return temp;
}
	
Inventory::Node* Inventory::rsearch(Node* temp,const string& name) const{
	if(temp==nullptr)
		return nullptr;

	if(name==temp->data.get_name())
		return temp;
	if(name<temp->data.get_name())
		return rsearch(temp->left,name);
	else
		return rsearch(temp->right,name);
}

void Inventory::insert(const Item& item){
	root= rinsert(root,item);
}

bool Inventory::contains(const string& name) const{
	if(rsearch(root,name)==nullptr)
		return false;
	else
		return true;
}

int Inventory::get_item_count(const string& name) const{
	Node* exists = rsearch(root,name);

	if(exists==nullptr)
		return 0;
	else
		return exists->count;
}

void Inventory::print() const{
	rprint(root);
}

void Inventory::add_pokecoins(int amount){
	if(amount>0)
		pokecoins= pokecoins+amount;
}

bool Inventory::spend_coins(int amount){
	if (amount>pokecoins ||  amount<0)
		return false;
	
	pokecoins = pokecoins- amount;
	return true;
}

bool Inventory::buy(const Item& item){
	if (!spend_coins(item.get_cost()))
		return false;

	insert(item);//size adjusted
	return true;
}

bool Inventory::sell(const string& item){
	Node* exists = rsearch(root, item);

	if(exists==nullptr)
		return false;
	if(exists->data.is_special())
		return false;

	int amount = exists->data.get_cost();

	if(!remove(item))//size adjusted
		return false;

	add_pokecoins(amount);
	return true;
}

void Inventory::clear(){
	extinguish(root);
	root=nullptr;
	size=0;
	}

bool Inventory::get_item_copy(const string& name, Item& item) const{
	Node* exists=rsearch(root,name);

	if(exists==nullptr)
		return false;

	item=exists->data;
	return true;
}


bool Inventory::drop(const string& item){
	Node* exists = rsearch(root, item);

	if(exists==nullptr)
		return false;
	if(exists->data.is_special())
		return false;

	return remove(item);//size adjusted
}

bool Inventory::remove(const string& name){
	Node* parent=nullptr;
	Node* currentnode = root;

	while(currentnode!=nullptr && currentnode->data.get_name()!=name){//will stop when reaches nullptr or of the names match to find where the node with the name is
		parent=currentnode;

		if(name>currentnode->data.get_name())
			currentnode=currentnode->right;
		else
			currentnode= currentnode->left;
	}

	if(currentnode==nullptr)
		return false;

	if(currentnode->count >1){
		currentnode->count--;
		return true;
	}

	if (currentnode->left != nullptr && currentnode->right !=nullptr){
		Node* parent2=currentnode;
		Node* nextup = currentnode->right;


		while(true){
			if (nextup->left == nullptr)
				break;
			parent2=nextup;
			nextup = nextup->left;
		}
		currentnode->data = nextup->data;
		currentnode->count= 1;

		currentnode=nextup;
		parent=parent2;

		if(currentnode->count>1){
			currentnode->count--;
			return true;
		}

	}

	Node* kid;
	if(currentnode->left !=nullptr)
		kid=currentnode->left;
	else
		kid=currentnode->right;

	if (parent==nullptr)
		root=kid;
	else if(parent->left==currentnode)
		parent->left=kid;
	else parent->right= kid;

	delete currentnode;
	size--;
	return true;
}

int Inventory::get_size() const{
	return size;
}

int Inventory::get_coins() const{
	return pokecoins;
}


	
