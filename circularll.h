#pragma once
#include "actor.h"
#include "Bridges.h"
#include "CircSLelement.h"


//What is the function of the boolean that returns true/false for each actor loop in the CLL
class CLL {
	private:
		struct Node {
			Actor* actorPTR = nullptr;
			Node* next = nullptr;
			Node* prev = nullptr;
		};
		Node* head = nullptr;
		bridges::CircSLelement<Actor*> bHead;
		bridges::CircSLelement<Actor*> bTail;
		//Node* tail = nullptr;
		unsigned int size{};
		Node* pointy{};
	public:
		bridges::Bridges* bridgesCSLL = new bridges::Bridges (3, "mediumrare", "132361449630");
		pair<Actor*, bool> current();	
		void reset_current_to_start();
		void list_insert(Actor* a);
		void list_delete(Actor* a);
		void output_bridges();
};
