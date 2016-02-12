// Chris Funaki
// Basketball Simulation Game

#include <iostream>
#include "Team.h"
#include "Game.h"

int main() {
	cout << "Welcome to the basketball simulation game!" << endl;

	Team home, away;
	// Get home and away team names
	cout << "What is the name of the home team? ";
	string name;
	cin >> name;
	home.setName(name);
	cout << "What is the name of the away team? ";
	cin >> name;
	away.setName(name);
	// Load team skills
	home.loadSkills();
	away.loadSkills();

	// Loads the game
	Game game(home, away);
	game.calcAllPcts();

	game.playQuarter();
	
	return 0;
}