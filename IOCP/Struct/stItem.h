#pragma once

#include <string>
#include <map>

// ��� ������ ����ü
struct Drop_ItemInfo
{
	int ItemID;
	int ItemType;
	std::string ItemName;
	std::string ItemExplan;
};

// ���� ��� ������ ����ü
struct mat_ItemInfo
{
	int ItemID;
	int ItemCount;
	int ItemType;
	std::string ItemName;
	std::string ItemExplan;
};

// ���� ������ ����ü
struct CB_ItemInfo
{
	int ItemID;
	int OccupationID;
	int ItemType;

	std::string ItemName;
	std::string ItemExplan;

	std::map<int, mat_ItemInfo> MatInfo;
};

// ��� ������ ����ü
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

// ��� ������ ����ü
struct Use_ItemInfo {
	int ItemID;
	int Fatigue;
};

// ���� ������ ����ü
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