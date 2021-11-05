#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"

#include "../Struct/stItem.h"

class DataUseItem
{
private:
	int ID;
	int Fatigue;
	
public:
	// Set
	void SetID(int nID) { ID = nID; }
	void SetFatigue(int nFatigue) { Fatigue = nFatigue; }

	// Get
	int GetID() { return ID; }
	int GetFatigue() { return Fatigue; }

};

class DataUseItems
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataUseItem*> UseItem_Data;

public:
	bool parse(std::string url);
};