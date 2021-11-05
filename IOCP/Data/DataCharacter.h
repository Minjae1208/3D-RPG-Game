#pragma once

#include <map>
#include <string>

#include "tinyxml2.h"

class DataCharacter
{
private :
	int Experience;
	int Fatigue;

public:
	// Set
	void SetExperience(int nExperience) { Experience = nExperience; }
	void SetFatigue(int nFatigue) { Fatigue = nFatigue; }

	// Get
	int GetExpereience() { return Experience; }
	int GetFatigue() { return Fatigue; }
};

class DataCharacters
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataCharacter*> Character_Data;

public:
	bool parse(std::string url);
};