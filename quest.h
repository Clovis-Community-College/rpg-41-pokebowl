#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <unordered_map>
#include <ncurses.h>

using namespace std;

class QuestSystem{
	private:
		unordered_map<int, string> quests;
		unordered_map<int, bool> done;

	public:
		QuestSystem();

		void boss_killed();

		void draw(int max_x);
};

#endif
