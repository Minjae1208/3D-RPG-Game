#pragma once

#include <string>
#include <map>
#include "tinyxml2.h"

#include "../Struct/stItem.h"

class DataDropItem
{
private:
	int ID;
	int Type;
	std::string Name;
	std::string Explan;

public:
	// Set
	void SetID(int nID) { ID = nID; }
	void SetType(int nType) { Type = nType; }
	void SetName(std::string strName) { Name = strName; }
	void SetExplan(std::string strExplan) { Explan = strExplan; }

	// Get
	int GetID() { return ID; }
	int GetType() { return Type; }
	std::string GetName() { return Name; }
	std::string GetExplan() { return Explan; }

	void GetAllData(struct Drop_ItemInfo * kDrop_ItemInfo);
};

class DataDropItems
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataDropItem*> DropItem_Data;

public:
	bool parse(std::string url);
};