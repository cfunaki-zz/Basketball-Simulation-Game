#include "Team.h"
#include <fstream>
#include <iostream>

// Sets the name of the team
void Team::setName(string name) {
	mName = name;
}

// Returns the name of a team
string Team::getName() {
	return mName;
}

// Loads in the skills of a team from csv file to the team's map of skills
void Team::loadSkills() {
	string directory = "..\\CSV Files\\";
	string fileName = directory + mName + "Skills.csv";
	ifstream skills(fileName);
	if (skills.is_open()) {
		while (!skills.eof()) {
			// Grab each line and put it in the map
			string line;
			getline(skills, line);
			string key = line.substr(0, line.find(","));
			string strValue = line.substr(line.find(",") + 1);
			int value = stoi(strValue);
			mSkills.insert(make_pair(key, value));
		}
	}
	else {
		cout << "Error! Could not open the file" << endl;
	}
}

// Returns the value corresponding to a skill
int Team::getSkill(string skill) {
	return mSkills.at(skill);
}

// Returns the value corresponding to a tactic
int Team::getTactic(string tactic) {
	return mTactics.at(tactic);
}

// Gets the map of skills
map<string, int> Team::getSkillMap() {
	return mSkills;
}

// Gets the map of tactics
map<string, int> Team::getTacticMap() {
	return mTactics;
}