#pragma once

#include <map>
#include <string>

#include "tinyxml2.h"
#include "DataCBMat.h"
#include "DataCBFirst.h"

#include "../Struct/stItem.h"

class DataCBSecond
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
	void SetID(int nID)								{ ID = nID; }
	void SetType(int nType)							{ Type = nType; }
	void SetName(std::string strName)				{ Name = strName; }
	void SetExplan(std::string strExplan)			{ Explan = strExplan; }
	void SetMatData(int index, DataCBMat cDataCBMat) { mat_Data[index] = cDataCBMat; }

	// Get
	int GetID()					{ return ID; }
	int GetType()				{ return Type; }
	std::string GetName()		{ return Name; }
	std::string GetExplan()		{ return Explan; }

	void GetAllData(struct CB_ItemInfo * kCB_ItemInfo);
};

class DataCBSeconds
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataCBSecond*> CBSecond_Data;

public:
	bool parse(std::string url, DataCBFirsts *  pCBFirst);
};