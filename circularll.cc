#include "circularll.h"
#include <CircSLelement.h>

using namespace bridges;
using namespace std;

void CLL::mark_pointy() {
	if (!bPointy) return;
	bPointy->setColor("Red");
	bPointy->getLinkVisualizer(bPointy->getNext())->setColor("Red");
	bPointy->getLinkVisualizer(bPointy->getNext())->setThickness(2.0f);
}

void CLL::output_bridges() {
	bridgesCLL->setDataStructure(bHead);
	bridgesCLL->visualize();
}

void CLL::list_insert(Actor* a) {
	if (!head) {
	/*
	head = head->prev = new Node{a, };
	size = 1;
	head->next = head->prev;
	head->prev->next = head;
	return;
	*/
	
	head = new Node{ a};
	head->next = head;
	head->prev = head;
	//head = new Node{ a};
	//head->next = head;
	//head->prev = head;
	pointy = head;
	size = 1;
	return;
	}
	if (head == head->prev) {
		head->prev = new Node{ a , head, head };
		head->next = head->prev;
		size++;
		return;
	}
	if (head != head->prev) {
		head->prev->next = new Node{ a , head, head->prev};
		head->prev = head->prev->next;
		//tail->next = head;
		size++;
	}

	/* OLD METHOD
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
	*/
}

void CLL::reset_current_to_start() {
	pointy = head;
}

pair<Actor*, bool> CLL::current() {
	Actor* tmpActor = pointy->actorPTR;
	if (pointy == head->prev) {	
		reset_current_to_start();
		pair<Actor*, bool> loopResult = {tmpActor, true};
		return loopResult;
	}
	pointy = pointy->next;
	pair<Actor*, bool> result = {tmpActor, false};
	return result;
}

void CLL::list_delete(Actor* a) {
if (!size) {//empty
	cout << "empty. yup\n";
		return;
} 
if (size == 1 && head->actorPTR == a) { //deleting from size 1
	delete head;
	pointy = nullptr;
	head = nullptr;
	size = 0;
	return;
}
Node* tmp = head;
if (head->actorPTR == a) {
	if (pointy == head) {
		pointy = pointy->next;
	}
	head->prev->next = head->next;
	head->next->prev = head->prev;
	head = head->next;
	size--;
	delete tmp;
	
	return;
}
tmp = tmp->next;
while (1) {

	if (tmp->actorPTR == a) {
		tmp->prev->next = tmp->next; //link node before deleted one to next one
		tmp->next->prev = tmp->prev;
		if (tmp == pointy) {
			pointy = pointy->next;
		}
		delete tmp;
		size--;
		return;
	}
	if (tmp == head) return;
	tmp = tmp->next;
}

	/*
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
	}*/
}

void CLL::output_bridges() {
	bridgesCSLL->visualize();
}
