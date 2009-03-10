#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
using std::vector;

class Scoreboard {
	public:
		vector<int> kills; // player numbers of the kills (in order) -- -1 is a SD
		vector<int> deaths; // player numbers of the deaths (in order)
		int playercount; // total number of players
		Scoreboard(int count); // creates a scoreboard
		void addDeath(int killer, int killed); // registers a kill
		void clear(); // clears the score
		int getKills(int player); // the number of kills by player with number player
		int getDeaths(int player); // the number of deaths by player with number player
		int getSDs(int player); // the number of SDs by player with number player
		int getTotal(int player); // totals score for a player with number player
}; // tracks all scores

#endif
