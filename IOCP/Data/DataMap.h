#pragma once

#include <string>
#include <map>
#include "tinyxml2.h"

#include "DataInteraction.h"
#include "../Manager/LevelManager.h"

class DataMap
{
public:
	int ID;
	std::string NameID;
	std::string Name;
	int Type;
	int Interaction;
};

class DataMaps
{
public:
	tinyxml2::XMLDocument doc;
	DataInteraction* Interactions = new DataInteraction();
	std::map<int, DataMap*> Map_Data;

public:
	bool parse(std::string url, std::map<int, Level*> & Levels, std::map<int, DataLevel*> & DataLevels);
};