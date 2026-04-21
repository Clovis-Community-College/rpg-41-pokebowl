#include "quest.h"
#include "inventory.h"
#include <ncurses.h>

QuestSystem::QuestSystem(){
	quests[1] = "Kill Monster, bring back the key..";
	quests[2] = "Bring back the Eye..";
	quests[3] = "TBA";

	done[1] = false;
	done[2] = false;
	done[3] = false;
}

void QuestSystem::boss_killed(){
	done[1] = true;
	done[2] = true;
}

vector<string> QuestSystem::check_items(Inventory& inv){
	vector<string> completed;
	if (!done[1] && inv.contains("Monster Key")) {
		inv.remove("Monster Key");
		done[1] = true;
		completed.push_back(quests[1]);
	}
	if (!done[2] && inv.contains("Ancient Eye")) {
		inv.remove("Ancient Eye");
		done[2] = true;
		completed.push_back(quests[2]);
	}
	return completed;
}

void QuestSystem::draw(int max_x){
	int bd = max_x - 38;

	attron(COLOR_PAIR(5));

	mvprintw(1, bd, "===============");
	mvprintw(2, bd, "   QUEST LOG   ");
	mvprintw(3, bd, "===============");

	for(int i = 1; i <= 3; i++){
		if(done[i]){
			mvprintw(3 + i*2, bd, "Quest %d: [DONE]", i);
		}
		else {
			mvprintw(3 + i*2, bd, "Quest %d: [ACTIVE]", i);
		}

		mvprintw(4 + i*2, bd, "%s", quests[i].c_str());
	}

	attroff(COLOR_PAIR(5));
}

