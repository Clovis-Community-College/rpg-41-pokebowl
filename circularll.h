#pragma once
#include "actor.h"

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
		void list_insert(Actor* a);
};
