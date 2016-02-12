#pragma once
#include <string>
#include <map>
using namespace std;

class Team {
private:
	string mName;
	map<string, int> mSkills;
	map<string, int> mTactics;
public:
	void setName(string name);
	string getName();
	void loadSkills();
	int getSkill(string skill);
	int getTactic(string tactic);
	map<string, int> getSkillMap();
	map<string, int> getTacticMap();
};