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
	int roll = rand()% 21;
	if (roll==0)
		return Item("Pika's Lightning Rod","weapon",50,0,45,false);
	else if (roll==1)
		return Item("Blastoise's Water Spit","weapon",50,0,35,false);
	else if (roll==2)
		return Item("Small Healing Potion","heal",10,20,0,false);
	else if (roll==3)
		return Item("Big Healing Potion","heal",20,50,0,false);
	else if (roll==4)
		return Item("Charizard's Fireball","weapon",20,0,50,false);
	else if (roll==5)
		return Item("Mewtwo's Psychic Wand","weapon",15,0,40,false);
	else if (roll==6)
		return Item("Poke Berry","heal",5,10,0,false);
	else if (roll==7)
		return Item("Snorlax's Scratch","weapon",15,0,25,false);
	else if (roll==8)
		return Item("Bulbasaur's Vine Whip","weapon",15,0,25,false);
	else if (roll==9)
		return Item("Meowth's Punch","weapon",5,0,5,false);
	else if (roll==10)
		return Item("Poke banana","heal",5,15,0,false);
	else if (roll==11)
		return Item("Mega Heal", "heal",25,60,0,false);
	else if (roll==12)
		return Item("Gyrados's Water Tornado", "weapon",40,0,45,false);
	else if (roll==13)
		return Item("Lucario's Aura Punch", "weapon",70,0,65,false);
	else if (roll==14)
		return Item("Citrus Fruit", "heal",10,20,0,false);
	else if (roll==15)
		return Item("Healing Potion", "heal",15,35,0,false);
	else if (roll==16)
		return Item("Raichu's Thunder Strike", "weapon",50,0,50,false);
	else if (roll==17)
		return Item("Tyranitaur's Smash", "weapon",50,0,50,false);
	else if (roll==18)
		return Item("Greninja Water Shuriken", "weapon",25,0,30,false);
	else if (roll==19)
		return Item("Arcanine Flame Burst", "weapon",25,0,30,false);
	else 
		return Item("Dragonite Slam", "weapon",30,0,40,false);
	
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

Inventory& Inventory::operator=(const Inventory& other){
	if(this==&other)
		return *this;

	clear();
	pokecoins=other.pokecoins;

	vector<Item> copi;
	other.get_all_items(copi);

	for(const Item& item:copi)
		insert(item);

	return *this;
}

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

//COmplete for inventory drop
void Inventory::collect_items(Node* temp, vector<Item>& list) const{
	if(temp==nullptr)
		return;
	collect_items(temp->left,list);
	
	for(int i=0;i<temp->count;i++){
		list.push_back(temp->data);
	}

	collect_items(temp->right,list);
}

void Inventory::get_all_items(vector<Item>& list) const{
	collect_items(root,list);
}

vector<Item> Inventory::drop_all(){
	vector<Item> kept_items;
	vector<Item> dropped_items;
	vector<Item> all_items;
	get_all_items(all_items);

	for(const Item& item: all_items){
		if(item.is_special())
			kept_items.push_back(item);
		else {
			dropped_items.push_back(item);
		}
	}
	clear();

	for(const Item& item: kept_items)
		insert(item);

	return dropped_items;
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


//for picking up purposes
void Inventory::pickup_all(const vector<Item>& vec){
	for(const Item& x: vec){
		insert(x);
	}
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

void Inventory::merchant_operation(Inventory& merchant){
		int choice;
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

bool Inventory::buy(Inventory& merchant, const string& item_name){
	Item item;
	if(!merchant.get_item_copy(item_name,item))
		return false;

	if(!spend_coins(item.get_cost()))
		return false;

	if(!merchant.remove(item_name)){
		add_pokecoins(item.get_cost());//to make sure if it fails, then pokecoins are readded
		return false;
	}

	insert(item);

	return true;
}

bool Inventory::sell(Inventory& merchant, const string& item_name){
	Node* exists = rsearch(root, item_name);

	if(exists==nullptr)
		return false;
	if(exists->data.is_special())
		return false;
	Item item=exists->data;
	int amount = exists->data.get_cost();

	if(!remove(item_name))//size adjusted
		return false;

	add_pokecoins(amount);
	merchant.insert(item);
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
		currentnode->count= nextup->count;

		currentnode=nextup;
		parent=parent2;

		currentnode->count=1;
		

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


	
