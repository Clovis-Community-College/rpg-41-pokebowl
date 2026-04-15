#include "circularll.h"
#include <CircSLelement.h>

using namespace bridges;

void CLL::list_insert(Actor* a) {
	if (!head) {
		head = tail = new Node{a};
		//CircSLelement<Actor*>* e1 = new CircSLelement<Actor*>(a);	
		size = 1;
		return;
	}
	if (head == tail || head != tail) {
		tail->next = new Node{a};
		tail = tail->next;
		tail->next = head;
		size++;
		return;
	}
	else {
		cerr << "circularll.h: list_insert() error";
		exit(EXIT_FAILURE);
	}
}

void CLL::list_delete(Actor* a) {
	if (!size) return;
	if (size == 1 && head->attacker == a) {
		delete head;
		tail = head = tail->next = head->next = nullptr;
		size = 0;
		return;
	}

	Node* tmp = head;
	if (tmp->attacker == a) {
		Node* linker = tmp;
		linker = linker->next;
		tail->next = linker;
		delete tmp;
	}
}

void CLL::output_bridges() {
	bridgesCSLL->visualize();
}
