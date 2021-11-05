#pragma once

#include <string>
#include <map>

// 드롭 아이템 구조체
struct Drop_ItemInfo
{
	int ItemID;
	int ItemType;
	std::string ItemName;
	std::string ItemExplan;
};

// 조합 재료 아이템 구조체
struct mat_ItemInfo
{
	int ItemID;
	int ItemCount;
	int ItemType;
	std::string ItemName;
	std::string ItemExplan;
};

// 조합 아이템 구조체
struct CB_ItemInfo
{
	int ItemID;
	int OccupationID;
	int ItemType;

	std::string ItemName;
	std::string ItemExplan;

	std::map<int, mat_ItemInfo> MatInfo;
};

// 장비 아이템 구조체
struct Equipment_ItemInfo
{
	int ItemID;
	int OccupationID;
	int ItemType;
	int EquipmentType;
	std::string ItemName;
	std::string ItemExplan;
	int Stat;
};

// 사용 아이템 구조체
struct Use_ItemInfo {
	int ItemID;
	int Fatigue;
};

// 상점 아이템 구조체
struct Shop_ItemInfo
{
	int BuyItemID;
	int NeedItemID;
	int NeedItemCount;
};

enum ItemType {
	ETC,
	CONSUME,
	EQUIPEMENT,
};

enum EquipmentSlotType {
	HAT,
	CLOTH,
	BOOTS,
	WEAPON,
	BAG,
};

enum EOccupation
{
	GATHERER,
	MINOR,
	HUNTER,
};