#pragma once
#include "actor.h"
#include "Bridges.h"
#include "CircSLelement.h"

class CLL {
	private:
		struct Node {
			Actor* attacker = nullptr;
			Node* next = nullptr;
		};
		Node* head = nullptr;
		Node* tail = nullptr;
		unsigned int size{};
	public:
		bridges::Bridges* bridgesCSLL = new bridges::Bridges (3, "mediumrare", "132361449630");
		void list_insert(Actor* a);
};
