#ifndef QUEST_H
#define QUEST_H

#include <string>
#include <vector>
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
		vector<string> check_items(class Inventory& inv);

		void draw(int max_x);
};

#endif
