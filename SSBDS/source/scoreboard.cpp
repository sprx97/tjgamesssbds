#include "scoreboard.h"
#include <vector>
using std::vector;

Scoreboard::Scoreboard(int count) {
	playercount = count;
} // creates a scoreboard
void Scoreboard::addDeath(int killer, int killed) {
	kills.push_back(killer);
	deaths.push_back(killed);
} // registers a kill
void Scoreboard::clear() {
	kills.clear();
	deaths.clear();
} // clears the score
int Scoreboard::getKills(int player) {
	int count = 0;
	for(int n = 0; n < (int)kills.size(); n++) {
		if(kills[n] == player) count++;
	}
	return count;
} // the number of kills by player with number player
int Scoreboard::getDeaths(int player) {
	int count = 0;
	for(int n = 0; n < (int)deaths.size(); n++) {
		if(deaths[n] == player && kills[n] != -1) count++;
	}
	return count;
} // the number of deaths by player with number player
int Scoreboard::getSDs(int player) {
	int count = 0;
	for(int n = 0; n < (int)deaths.size(); n++) {
		if(deaths[n] == player && kills[n] == -1) count++;
	}
	return count;
} // the number of SDs by player with number player
int Scoreboard::getTotal(int player) {
	int count = 0;
	for(int n = 0; n < (int)deaths.size(); n++) {
		if(kills[n] == player) count++;
		if(deaths[n] == player) count--;
	}
	return count;
} // totals score for a player with number player
