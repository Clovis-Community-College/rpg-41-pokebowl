#include "circularll.h"
#include <CircDLelement.h>
#include <CircSLelement.h>
#include <sstream>

using namespace bridges;
using namespace std;

void CLL::mark_pointy() {
	if (!bPointy) return;
	bPointy->setColor("magenta");
	bPointy->getLinkVisualizer(bPointy->getNext())->setColor("magenta");
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
		
		if (a->type() == "monster") bHead->setShape(SQUARE); 
		bHead->setColor("lime");	

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

		toLinkup->getLinkVisualizer(bHead)->setColor("lime");
		
		if (a->type() == "monster") {
			toLinkup->setShape(SQUARE);
		} 
		toLinkup->setColor("lime");
		bHead->getLinkVisualizer(toLinkup)->setColor("lime");
		size++;
		return;
	}
	if (head != head->prev) {
		bHead->getPrev()->getLinkVisualizer(bHead)->setColor("orange");

		head->prev->next = new Node{ a , head, head->prev};
		head->prev = head->prev->next;
		//tail->next = head;
		
		CircDLelement<string>* toLinkup = new CircDLelement<string>(NodeLabel.str(),NodeLabel.str());

		toLinkup->setNext(bHead);
		toLinkup->setPrev(bHead->getPrev());

		toLinkup->getLinkVisualizer(bHead->getPrev())->setColor("white");
		
		bHead->getPrev()->setNext(toLinkup);
		bHead->setPrev(bHead->getPrev()->getNext());

		if (a->type() == "monster") toLinkup->setShape(SQUARE);
		
		toLinkup->getPrev()->getLinkVisualizer(toLinkup)->setColor("lime");
		toLinkup->setColor("lime");
		
		bHead->getLinkVisualizer(bHead->getPrev())->setColor("white");
		bHead->getPrev()->getLinkVisualizer(bHead)->setColor("lime");

		size++;
	}
}

void CLL::reset_current_to_start() {
	pointy = head;
	bPointy = bHead;
}

pair<Actor*, bool> CLL::current() {
	Actor* tmpActor = pointy->actorPTR;
	if (pointy == head->prev) {	
		reset_current_to_start();
		pair<Actor*, bool> loopResult = {tmpActor, true};
		return loopResult;
	}
	pointy = pointy->next;
	bPointy = bPointy->getNext();
	pair<Actor*, bool> result = {tmpActor, false};
	return result;
}

void CLL::list_delete(Actor* a) {
if (!size) return; 
if (size == 1 && head->actorPTR == a) { //deleting from size 1
	delete head;
	delete bHead;
	pointy = nullptr;
	bPointy = nullptr;
	head = nullptr;
	bHead = nullptr;
	size = 0;
	return;
}
Node* tmp = head;
CircDLelement<string> *bTmp = bHead;
if (head->actorPTR == a) {
	if (pointy == head) {
		pointy = pointy->next;
		bPointy = bPointy->getNext();
	}
	head->prev->next = head->next;
	head->next->prev = head->prev;
	head = head->next;

	bHead->getPrev()->setNext(bHead->getNext());
	bHead->getNext()->setPrev(bHead->getPrev());
	bHead = bHead->getNext();

	bHead->getPrev()->getLinkVisualizer(bHead)->setColor("lime");

	bHead->getLinkVisualizer(bHead->getPrev())->setColor("white");
	bHead->setSize(20);

	size--;
	delete tmp;
	delete bTmp;
	
	return;
}
tmp = tmp->next;
bTmp = bTmp->getNext();
while (1) {
	if (tmp->actorPTR == a) {
		tmp->prev->next = tmp->next; //link node before deleted one to next one
		tmp->next->prev = tmp->prev;
		
		bTmp->getPrev()->setNext(bTmp->getNext());
		bTmp->getNext()->setPrev(bTmp->getPrev());

		bTmp->getPrev()->getLinkVisualizer(bTmp->getNext())->setColor("lime");
		bTmp->getNext()->getLinkVisualizer(bTmp->getPrev())->setColor("white");

		if (tmp == pointy) {
			pointy = pointy->next;
			bPointy = bPointy->getNext();
		}
		delete tmp;
		delete bTmp;
		size--;
		return;
	}
	if (tmp == head) return;
		tmp = tmp->next;
		bTmp = bTmp->getNext();
	}
}

CLL::~CLL() {
	delete bridgesCLL;
	if (!head) return;
	Node* whereWeAt = head->next;
	while (whereWeAt != head) {
		Node* tmp = whereWeAt;
		whereWeAt = whereWeAt->next;
		delete tmp;
	}
	delete head;
}
