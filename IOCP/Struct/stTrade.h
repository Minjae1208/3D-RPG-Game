#pragma once

#include <map>

// ��ȯ ������ ����ü
struct TradeItemInfo {
	int ItemCount;
	int InventorySlot;
	int ItemType;
};

// ��ȯ ���� ����ü
struct TradeInfo {

	int SessionID;
	// ItemID / ItemCount
	std::map<int, struct TradeItemInfo*> OnPutItem;

	int RecentSlotNumber;
	bool Accept;
};