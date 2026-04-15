#pragma once
#include "actor.h"
#include "Bridges.h"
#include "CircSLelement.h"


//What is the function of the boolean that returns true/false for each actor loop in the CLL
class CLL {
	private:
		struct Node {
			Actor* actorPointer = nullptr;
			Node* next = nullptr;
		};
		Node* head = nullptr;
		bridges::CircSLelement<Actor*> bHead;
		bridges::CircSLelement<Actor*> bTail;
		Node* tail = nullptr;
		unsigned int size{};
	public:
		bridges::Bridges* bridgesCSLL = new bridges::Bridges (3, "mediumrare", "132361449630");
		
		void list_insert(Actor* a);
		void list_delete(Actor* a);
		void output_bridges();
};
