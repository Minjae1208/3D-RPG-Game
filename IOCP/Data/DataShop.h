#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"

#include "../Struct/stItem.h"

class DataShop
{
private:
	int BuyItemID;
	int NeedItemID;
	int NeedItemCount;

public:
	// Set
	void SetBuyItemID(int nBuyItemID) { BuyItemID = nBuyItemID; }
	void SetNeedItemID(int nNeedItemID) { NeedItemID = nNeedItemID; }
	void SetNeedItemCount(int nNeedItemCount) { NeedItemCount = nNeedItemCount; }

	// Get
	int GetBuyItemID() { return BuyItemID; }
	int GetNeedItemID() { return NeedItemID; }
	int GetNeedItemCount() { return NeedItemCount; }

	void GetAllData(Shop_ItemInfo * kShop_ItemInfo);
};

class DataShops
{
public:
	tinyxml2::XMLDocument doc;
	std::map<int, DataShop*> HunterShop_Data;
	std::map<int, DataShop*> GathererShop_Data;
	std::map<int, DataShop*> MinorShop_Data;

public:
	bool parse(std::string url);
};