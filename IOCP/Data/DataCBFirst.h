#pragma once

#include <map>
#include <queue>
#include <string>

#include "tinyxml2.h"
#include "DataCBMat.h"
#include "DataDropItem.h"

#include "../Struct/stItem.h"

class DataCBFirst
{
private:
	int ID;
	int Type;
	std::string Name;
	std::string Explan;
	
public:
	std::map<int, DataCBMat> mat_Data;

public:
	// Set
	void SetID(int nID)									{ ID = nID; }
	void SetType(int nType)								{ Type = nType; }
	void SetName(std::string strName)					{ Name = strName; }
	void SetExplan(std::string strExplan)				{ Explan = strExplan; }
	void SetMatData(int index, DataCBMat cDataCBMat)	{ mat_Data[index] = cDataCBMat; }

	// Get
	int GetID()					{ return ID; }
	int GetType()				{ return Type; }
	std::string GetName()		{ return Name; }
	std::string GetExplan()		{ return Explan; }

	void GetAllData(struct CB_ItemInfo * kCB_ItemInfo);
};

class DataCBFirsts
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataCBFirst*> CBFirst_Data;

public:
	bool parse(std::string url, DataDropItems * pDropItem);
};