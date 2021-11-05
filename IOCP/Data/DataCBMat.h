#pragma once

#include <string>

#include "../Struct/stItem.h"

class DataCBMat
{
private:
	int ID;
	int Count;
	int Type;
	std::string Name;
	std::string Explan;
	
public:
	// Set
	void SetID(int nID) { ID = nID; }
	void SetCount(int nCount) { Count = nCount; }
	void SetType(int nType) { Type = nType; }
	void SetName(std::string strName) { Name = strName; }
	void SetExplan(std::string strExplan) { Explan = strExplan; }

	// Get
	void GetAllData(struct mat_ItemInfo * kmat_ItemInfo)
	{
		kmat_ItemInfo->ItemID = ID;
		kmat_ItemInfo->ItemCount = Count;
		kmat_ItemInfo->ItemType = Type;
		kmat_ItemInfo->ItemName = Name;
		kmat_ItemInfo->ItemExplan = Explan;
	}

	int GetID() { return ID; }
	int GetCount() { return Count; }
	int GetType() { return Type; }
};