#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"

#include "../Struct/stItem.h"

class DataEquipment
{
private:
	int ID;
	int Type;
	int EQType;
	int Occupation;
	std::string Name;
	std::string Explan;
	int Stat;

public:
	// Set
	void SetID(int nID) { ID = nID; }
	void SetType(int nType) { Type = nType; }
	void SetEQType(int nEQType) { EQType = nEQType; }
	void SetOccupation(int nOccupation) { Occupation = nOccupation; }
	void SetName(std::string strName) { Name = strName; }
	void SetExplan(std::string strExplan) { Explan = strExplan; }
	void SetStat(int nStat) { Stat = nStat; }

	// Get
	int GetID() { return ID; }
	int GetType() { return Type; }
	int GetEQType() { return EQType; }
	int GetOccupation() { return Occupation; }
	std::string GetName() { return Name; }
	std::string GetExplan() { return Explan; }
	int GetStat() { return Stat; }

	void GetAllData(Equipment_ItemInfo * kEquipment_ItemInfo);
};


class DataEquipments
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataEquipment*> Equipment_Data;

public:
	bool parse(std::string url);
};