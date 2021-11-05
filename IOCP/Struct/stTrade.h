#pragma once

#include <map>

// 교환 아이템 구조체
struct TradeItemInfo {
	int ItemCount;
	int InventorySlot;
	int ItemType;
};

// 교환 정보 구조체
struct TradeInfo {

	int SessionID;
	// ItemID / ItemCount
	std::map<int, struct TradeItemInfo*> OnPutItem;

	int RecentSlotNumber;
	bool Accept;
};