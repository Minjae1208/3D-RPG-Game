#pragma once

#include <map>
#include <WinSock2.h>

#include "../Packet/PlayerPacket.h"


struct ItemInfo {
	int ItemID;
	int ItemCount;
	int SlotNumber;
	int ItemType;
	int ItemStat;
};

struct UserInventory {
	int MaxSize = 10;
	std::map<int, ItemInfo> Inventroy;
};

struct UserEquipment {
	std::map<int, ItemInfo> Equipment;
};

struct User {
	Player cPlayer;
	SOCKET sockUser;
	UserInventory stInventory;
	UserEquipment stEquipment;
};