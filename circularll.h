#pragma once
#include "actor.h"
#include "Bridges.h"
#include "CircDLelement.h"
#include <utility>


//What is the function of the boolean that returns true/false for each actor loop in the CLL
class CLL {
	private:
		struct Node {
			Actor* actorPTR = nullptr;
			Node* next = nullptr;
			Node* prev = nullptr;
			// removal of bellow was neccessary for code compilation
			// sry george ;(((
			// Node(Actor* a) : actorPTR(a), next(nullptr), prev(nullptr) {}
				
		};
		Node* head{};
		bridges::CircDLelement<string> *bHead = nullptr;

		//Node* tail = nullptr;
		unsigned int size{};
		Node* pointy{};
		bridges::CircDLelement<string> *bPointy = nullptr;
	public:
		CLL() : head(nullptr), size(0), pointy(nullptr) {}
		bridges::Bridges* bridgesCLL = new bridges::Bridges (3, "mediumrare", "132361449630");
		pair<Actor*, bool> current();	
		void reset_current_to_start();
		void list_insert(Actor* a);
		void list_delete(Actor* a);
		void output_bridges();
		void mark_pointy(); //marks current node pointy points to as red in Bridges visualization
		bridges::CircDLelement<string>* get_bHead() {return bHead;}
};
