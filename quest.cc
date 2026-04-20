#include "quest.h"
#include <ncurses.h>

QuestSystem::QuestSystem(){
	//hash map quest QuestSystem
	quests[1] = "Kill Monster, bring back the key..";
	quests[2] = "Bring back the Eye..";
	quests[3] = "TBA";

	// default false and active
	done[1] = false;
	done[2] = false;
	done[3] = false;
}

void QuestSystem::boss_killed(){
	done[1] = true;
	done[2] = true;
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

