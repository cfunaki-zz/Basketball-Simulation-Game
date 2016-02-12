#include "Game.h"
#include <iostream>
#include <fstream>

// Constructs the game, setting the teams, scores, and randomness
Game::Game(Team home, Team away) {
	mQuarter = 1;
	mHomeTeam = home;
	mAwayTeam = away;
	mHomeScore, mAwayScore = 0;
	mScore = &mHomeScore;

	// Prepare random number generator
	gen = new mt19937(rd());
	dist = new uniform_real_distribution<double>(0.0, 1.0);

	// Load in CalculatePct file
	ifstream pctFile("..\\CSV Files\\CalculatePct.csv");
	if (pctFile.is_open()) {
		int j = 0;
		int m, n;
		double tempDouble;
		string line, temp;
		while (!pctFile.eof()) {
			// Store each part of the calculation formula
			getline(pctFile, line);
			mEvents[j] = line.substr(0, line.find(","));
			m = line.find(",") + 1;
			for (int i = 0; i < 8; i++) {
				// Store each double in the vector within mCalc
				line = line.substr(m + 1);
				n = line.find(",");
				temp = line.substr(0, n);
				tempDouble = stod(temp);
				mCalc[mEvents[j]].push_back(tempDouble);
				m = n;
			}
			j++;
		}
	}
	else {
		cout << "Error! Could not open the file" << endl;
	}
}

// Destructor, deletes pointers
Game::~Game() {
	mScore = NULL;
	delete mScore;
}

// Calculates the probability of each event
double Game::calcPct(string s, map<string, int>& homeSkills, map<string, int>& awaySkills) {
	double playerSkill, opponentSkill, calcPct;
	playerSkill = (mCalc[s][2] * homeSkills["Offense"]) + (mCalc[s][3] * homeSkills["Athletics"]);
	opponentSkill = (mCalc[s][5] * awaySkills["Defense"]) + (mCalc[s][6] * awaySkills["Athletics"]);
	calcPct = mCalc[s][0] + mCalc[s][1] * (playerSkill - (mCalc[s][4] * opponentSkill) + mCalc[s][7]);
	if (calcPct < 0) {
		return 0;
	}
	else {
		return calcPct;
	}
}

// Calculates probabilities of all possible events
void Game::calcAllPcts() {

	for (string s : mEvents) {
		mHomePct[s] = calcPct(s, mHomeTeam.getSkillMap(), mAwayTeam.getSkillMap());
		mAwayPct[s] = calcPct(s, mAwayTeam.getSkillMap(), mHomeTeam.getSkillMap());
		}
}

// Gets probability from each team's map
double Game::getPct(string pct) {
	if (mHomePoss == true) {
		return mHomePct.at(pct);
	}
	else {
		return mAwayPct.at(pct);
	}
}

// Returns the home team's score
int Game::getHomeScore() {
	return mHomeScore;
}

// Returns the away team's score
int Game::getAwayScore() {
	return mAwayScore;
}

// Resets the scores to 0
void Game::resetScore() {
	mHomeScore = 0;
	mAwayScore = 0;
}

// Generates a random number between 0 and 1
double Game::randNum() {
	double x = dist->operator()(*gen);
	return x;
}

// Decides which type of shot is taken and if it scores
void Game::shot() {
	double x = randNum();
	double y = randNum();
	double z = randNum();
	mClock -= 12;

	do {
		x -= getPct("attThree");
		if (x < 0) {
			cout << "Three point shot" << endl;
			y -= getPct("blockThree");
			if (y < 0) {
				block();
				break;
			}
			else {
				z -= getPct("madeThree");
				if (z < 0) {
					cout << "Three point made" << endl;
					*mScore += 3;
					mHomePoss = !mHomePoss;
					break;
				}
				else {
					cout << "Three point missed" << endl;
					rebound();
					break;
				}
			}
		}
		x -= getPct("attMid");
		if (x < 0) {
			cout << "Midrange shot" << endl;
			y -= getPct("blockMid");
			if (y < 0) {
				block();
				break;
			}
			else {
				z -= getPct("madeMid");
				if (z < 0) {
					cout << "Midrange shot made" << endl;
					*mScore += 2;
					mHomePoss = !mHomePoss;
					break;
				}
				else {
					cout << "Midrange shot missed" << endl;
					rebound();
					break;
				}
			}
		}
		x -= getPct("attDrive");
		if (x < 0) {
			cout << "Driving layup" << endl;
			y -= getPct("blockDrive");
			if (y < 0) {
				block();
				break;
			}
			else {
				z -= getPct("madeDrive");
				if (z < 0) {
					cout << "Driving layup made" << endl;
					*mScore += 3;
					mHomePoss = !mHomePoss;
					break;
				}
				else {
					cout << "Driving layup missed" << endl;
					rebound();
					break;
				}
			}
		}
		else {
			cout << "Post shot" << endl;
			y -= getPct("blockPost");
			if (y < 0) {
				block();
				break;
			}
			else {
				z -= getPct("madePost");
				if (z < 0) {
					cout << "Post shot made" << endl;
					*mScore += 3;
					mHomePoss = !mHomePoss;
					break;
				}
				else {
					cout << "Post shot missed" << endl;
					rebound();
					break;
				}
			}
		}
	} while (x > 0);
}

// Decides the winner of a rebound
void Game::rebound() {
	double x = randNum();
	x -= getPct("rebound");
	if (x < 0) {
		cout << "Offensive rebound" << endl;
	}
	else {
		cout << "Defensive rebound" << endl;
		mHomePoss = !mHomePoss;
	}
}

// Blocked shot leads to chance of a fast break
void Game::block() {
	cout << "Blocked shot" << endl;
	double x = randNum();
	x -= .3;
	if (x < 0) {
		fastBreak();
	}
	mHomePoss = !mHomePoss;
}

// Steal off the dribble leads to chance of a fast break
void Game::stealDribble() {
	cout << "Stolen off the dribble" << endl;
	mHomePoss = !mHomePoss;
	double x = randNum();
	x -= .4;
	if (x < 0) {
		fastBreak();
	}
	mClock -= 12;
}

// Stolen pass leads to chance of a fast break
void Game::stealPass() {
	cout << "Pass intercepted" << endl;
	mHomePoss = !mHomePoss;
	double x = randNum();
	x -= .4;
	if (x < 0) {
		fastBreak();
	}
	mClock -= 12;
}

// Other turnover leads to deadball
void Game::otherTurnovers() {
	cout << "Turnover" << endl;
	mHomePoss = !mHomePoss;
	mClock -= 12;
}

// Defensive foul leads to chance of free throws
void Game::foulDef() {
	double x = randNum();
	cout << "Defensive foul" << endl;
	x -= .45;
	if (x < 0) {
		cout << "Free throws" << endl;
		freeThrow();
	}
	mClock -= 12;
}

// Offensive foul leads to turnover
void Game::foulOff() {
	cout << "Offensive foul" << endl;
	mHomePoss = !mHomePoss;
	mClock -= 12;
}

// Decides if free throws are scored
void Game::freeThrow() {
	double x = randNum();
	double y = randNum();
	x -= getPct("madeFreeThrow");
	if (x < 0) {
		cout << "First free throw made" << endl;
		*mScore += 1;
	}
	x -= getPct("madeFreeThrow");
	if (y < 0) {
		cout << "Second free throw made" << endl;
		*mScore += 1;
		mHomePoss = !mHomePoss;
	}
	else {
		cout << "Second free throw missed" << endl;
		rebound();
	}
}

// Decides if fast break scores
void Game::fastBreak() {
	cout << "Fast break" << endl;
	double x = randNum();
	x -= getPct("madeFastBreak");
	if (x < 0) {
		*mScore += 2;
		mHomePoss = !mHomePoss;
		cout << "Scored on the fast break" << endl;
	}
	else {
		cout << "Missed on the fast break" << endl;
		rebound();
	}
}

// Decides winner initial of jump ball
void Game::jumpBall() {
	double x = randNum();
	x -= .5;
	if (x < 0) {
		mHomePoss = true;
		cout << "Home team possession" << endl;
	}
	else {
		mHomePoss = false;
		cout << "Away team possession" << endl;
	}
}

// Runs through one possession
// Decides event: turnover, foul, shot
void Game::playPoss() {

	if (mHomePoss == true) {
		mScore = &mHomeScore;
	}
	else {
		mScore = &mAwayScore;
	}
	double x = randNum();

	do {
		x -= getPct("stealDribble");
		if (x < 0) {
			stealDribble();
			break;
		}
		x -= getPct("stealPass");
		if (x < 0) {
			stealPass();
			break;
		}
		x -= getPct("otherTurnovers");
		if (x < 0) {
			otherTurnovers();
			break;
		}
		x -= getPct("foulDef");
		if (x < 0) {
			foulDef();
			break;
		}
		x -= getPct("foulOff");
		if (x < 0) {
			foulOff();
			break;
		}
		else {
			shot();
			break;
		}
	} while (x > 0);
}

// Plays one quarter, looping possessions until clock hits 0
void Game::playQuarter() {
	mClock = 720;
	jumpBall();
	while (mClock > 0) {
		playPoss();
	}
	cout << "\n" << endl;
	cout << "Home score: " << mHomeScore << endl;
	cout << "Away score: " << mAwayScore << endl;
}

void Game::runSimulations() {
	cout << "How many simulations would you like to run? ";
	int n;
	cin >> n;

	int sumHome = 0;
	int sumAway = 0;
	double aveHome = 0;
	double aveAway = 0;


	for (int i = 0; i < n; i++) {
		playQuarter();
		sumHome += getHomeScore();
		sumAway += getAwayScore();
		resetScore();
	}

	aveHome = double(sumHome) / n;
	aveAway = double(sumAway) / n;
	cout << "Average home score: " << aveHome << endl;
	cout << "Average away score: " << aveAway << endl;
}

void Game::showPcts() {
	bool loop = true;
	while (loop = true) {
		cout << "Would you like to find a percentage? ";
		string answer;
		cin >> answer;
		if (answer == "y" || answer == "yes") {
			cout << "What percentage would you like to find? ";
			string percent;
			cin >> percent;
			mHomePoss = true;
			if (getPct(percent) >= 0) {
				cout << "Home " << percent << " percentage = " << getPct(percent) << endl;
				mHomePoss = false;
				cout << "Away " << percent << " percentage = " << getPct(percent) << endl;
			}
			else {
				cout << "Invalid entry" << endl;
			}
		}
		else {
			break;
		}
	}
}