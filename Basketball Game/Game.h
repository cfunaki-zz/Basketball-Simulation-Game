#pragma once
#include <string>
#include <random>
#include "Team.h"
using namespace std;

class Game {
private:
	Team mHomeTeam;
	Team mAwayTeam;
	bool mHomePoss; // true for home team possession
	int mHomeScore;
	int mAwayScore;
	int *mScore;
	int mQuarter;
	int mClock;
	string mEvents[21];
	map<string, vector<double>> mCalc;
	map<string, double> mHomePct;
	map<string, double> mAwayPct;
	random_device rd;
	mt19937* gen;
	uniform_real_distribution<double>* dist;
public:
	Game(Team home, Team away);
	~Game();
	//Calculations
	double calcPct(string s, map<string, int>& homeSkills, map<string, int>& awaySkills);
	void calcAllPcts();

	double getPct(string pct);
	int getHomeScore();
	int getAwayScore();
	double randNum();
	void resetScore();

	void shot();
	void rebound();
	void block();
	void stealPass();
	void stealDribble();
	void otherTurnovers();
	void foulDef();
	void foulOff();
	void freeThrow();
	void fastBreak();
	void jumpBall();

	void playPoss();
	void playQuarter();

	void runSimulations();
	void showPcts();
};