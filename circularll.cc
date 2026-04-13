#include "circularll.h"

void CLL::list_insert(Actor* a) {
	if (!head) {
		head = tail = new Node{a};
		return;
	}
	if (head == tail || head != tail) {
		tail->next = new Node{a};
		tail = tail->next;
		tail->next = head;
		return;
	}
	else {
		cerr << "circularll.h: list_insert() error";
		exit(EXIT_FAILURE);
	}
}
