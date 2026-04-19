#include "circularll.h"
#include <CircDLelement.h>
#include <CircSLelement.h>
#include <sstream>

using namespace bridges;
using namespace std;

void CLL::mark_pointy() {
	if (!bPointy) return;
	bPointy->setColor("Red");
	bPointy->getLinkVisualizer(bPointy->getNext())->setColor("Red");
	bPointy->getLinkVisualizer(bPointy->getNext())->setThickness(2.0f);
}

void CLL::output_bridges() {
	if (!bHead) return;
	mark_pointy();
	bridgesCLL->setDataStructure(bHead);
	bridgesCLL->visualize();
}

void CLL::list_insert(Actor* a) {
	stringstream NodeLabel;
	NodeLabel << a->name() << " {" << a->type() << ") - SPEED " << dynamic_cast<HasInitiative*>(a)->get_speed();
	if (!head) {
		head = new Node{ a};
		head->next = head;
		head->prev = head;
		pointy = head;
		
		bHead = new CircDLelement<string>(NodeLabel.str(), NodeLabel.str());	
		bHead->setNext(bHead);
		bHead->setPrev(bHead);
		bPointy = bHead;
		bHead->setSize(20);
		
		if (a->type() == "monster") {
			bHead->setShape(SQUARE);
			bHead->setColor("magenta");
		} else {
			bHead->setColor("lime");
		}

		size = 1;
		return;
	}
	if (head == head->prev) {
		head->prev = new Node{ a , head, head };
		head->next = head->prev;
		
		CircDLelement<string>* toLinkup = new CircDLelement<string>(NodeLabel.str(),NodeLabel.str());

		toLinkup->setNext(bHead);
		toLinkup->setPrev(bHead);

		bHead->setPrev(toLinkup);
		
		bHead->setNext(bHead->getPrev());

		toLinkup->getLinkVisualizer(bHead)->setColor("white");
		
		if (a->type() == "monster") {
			toLinkup->setShape(SQUARE);
			toLinkup->setColor("magenta");
			toLinkup->getLinkVisualizer(bHead)->setColor("magenta");
		} else {
			toLinkup->setColor("lime");
			toLinkup->getLinkVisualizer(bHead)->setColor("lime");
		}
		size++;
		return;
	}
	if (head != head->prev) {
		head->prev->next = new Node{ a , head, head->prev};
		head->prev = head->prev->next;
		//tail->next = head;
		
		CircDLelement<string>* toLinkup = new CircDLelement<string>(NodeLabel.str(),NodeLabel.str());

		toLinkup->setNext(bHead);
		toLinkup->setPrev(bHead->getPrev());

		toLinkup->getLinkVisualizer(bHead->getPrev())->setColor("white");
		
		bHead->getPrev()->setNext(toLinkup);
		bHead->setPrev(bHead->getPrev()->getNext());

		if (a->type() == "monster") {
			toLinkup->setShape(SQUARE);
			toLinkup->getPrev()->getLinkVisualizer(toLinkup)->setColor("magenta");
			toLinkup->setColor("magenta");
		} else {
			toLinkup->getPrev()->getLinkVisualizer(toLinkup)->setColor("lime");
			toLinkup->setColor("lime");
		}

		size++;
	}
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
