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
	//remove if/ else
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
	if (head->attacker == a) {
		tail->next = head->next;
		head = head->next;
		size--;
		delete tmp;
		return;
	}
	Node* prev = head;
	tmp = tmp->next;
	while (1) {
		if (tmp->attacker == a) {
			prev->next = tmp->next;
			if (tmp == tail) {
				tail = prev;
			}
			size--;
			delete tmp;
			return;
		}
		if (tmp == tail) return;
		tmp = tmp->next;
		prev = prev->next;
	}
}

void CLL::output_bridges() {
	bridgesCSLL->visualize();
}
