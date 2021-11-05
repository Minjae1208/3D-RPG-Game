#pragma once

#include "../Struct/stUser.h"

class Packet_XMLDropItem
{
public:
	std::map<int, Drop_ItemInfo> DropItems;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLDropItem& info)
	{
		stream << info.DropItems.size() << std::endl;
		for (auto& itetcItem : info.DropItems)
		{
			stream << itetcItem.second.ItemID << std::endl;
			stream << itetcItem.second.ItemType << std::endl;
			stream << itetcItem.second.ItemName << std::endl;
			stream << itetcItem.second.ItemExplan << std::endl;
		}
		return stream;
	}
	friend std::istream& operator>>(std::istream& stream, Packet_XMLDropItem& info)
	{
		int MapSize = 0;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			Drop_ItemInfo ketcItemInfo;

			stream >> ketcItemInfo.ItemID;
			stream >> ketcItemInfo.ItemType;
			stream >> ketcItemInfo.ItemName;
			stream >> ketcItemInfo.ItemExplan;

			info.DropItems[i] = ketcItemInfo;
		}
		return stream;
	}
};

// 수정 필요 First & Second
class Packet_XMLCB_First
{
public:
	std::map<int, CB_ItemInfo> CB_FirstItems;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLCB_First& info)
	{
		stream << info.CB_FirstItems.size() << std::endl;
		for (auto& itetcItem : info.CB_FirstItems)
		{
			stream << itetcItem.second.ItemID << std::endl;
			stream << itetcItem.second.ItemType << std::endl;
			stream << itetcItem.second.ItemName << std::endl;
			stream << itetcItem.second.ItemExplan << std::endl;

			stream << itetcItem.second.MatInfo.size() << std::endl;
			int size = itetcItem.second.MatInfo.size();
			for (int i = 0; i < size; i++)
			{
				stream << itetcItem.second.MatInfo[i].ItemID << std::endl;
				stream << itetcItem.second.MatInfo[i].ItemCount << std::endl;
				stream << itetcItem.second.MatInfo[i].ItemName << std::endl;
				stream << itetcItem.second.MatInfo[i].ItemType << std::endl;
				stream << itetcItem.second.MatInfo[i].ItemExplan << std::endl;
			}
		}
		return stream;
	}
	friend std::istream& operator>>(std::istream& stream, Packet_XMLCB_First& info)
	{
		int MapSize = 0;
		int CBItemCount;
		mat_ItemInfo Data;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			CB_ItemInfo ketcMakeItemInfo;

			stream >> ketcMakeItemInfo.ItemID;
			stream >> ketcMakeItemInfo.ItemType;
			stream >> ketcMakeItemInfo.ItemName;
			stream >> ketcMakeItemInfo.ItemExplan;

			stream >> CBItemCount;
			for (int j = 0; j < CBItemCount; j++)
			{
				stream >> Data.ItemID;
				stream >> Data.ItemCount;
				stream >> Data.ItemName;
				stream >> Data.ItemType;
				stream >> Data.ItemExplan;

				ketcMakeItemInfo.MatInfo[j] = Data;
			}
			info.CB_FirstItems[i] = ketcMakeItemInfo;
		}
		return stream;
	}
};

class Packet_XMLCB_Second
{
public:
	std::map<int, CB_ItemInfo> CB_SecondItems;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLCB_Second& info)
	{
		stream << info.CB_SecondItems.size() << std::endl;
		for (auto& itetcItem : info.CB_SecondItems)
		{
			stream << itetcItem.second.ItemID << std::endl;
			stream << itetcItem.second.ItemType << std::endl;
			stream << itetcItem.second.ItemName << std::endl;
			stream << itetcItem.second.ItemExplan << std::endl;

			stream << itetcItem.second.MatInfo.size() << std::endl;
			int size = itetcItem.second.MatInfo.size();
				for (int i = 0; i < size; i++)
				{
					stream << itetcItem.second.MatInfo[i].ItemID << std::endl;
					stream << itetcItem.second.MatInfo[i].ItemCount << std::endl;
					stream << itetcItem.second.MatInfo[i].ItemName << std::endl;
					stream << itetcItem.second.MatInfo[i].ItemType << std::endl;
					stream << itetcItem.second.MatInfo[i].ItemExplan << std::endl;
				}
		}
		return stream;
	}
	friend std::istream& operator>>(std::istream& stream, Packet_XMLCB_Second& info)
	{
		int MapSize = 0;
		int CBItemCount;
		mat_ItemInfo Data;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			CB_ItemInfo ketcMakeItemInfo;

			stream >> ketcMakeItemInfo.ItemID;
			stream >> ketcMakeItemInfo.ItemType;
			stream >> ketcMakeItemInfo.ItemName;
			stream >> ketcMakeItemInfo.ItemExplan;

			stream >> CBItemCount;
			for (int j = 0; j < CBItemCount; j++)
			{
				stream >> Data.ItemID;
				stream >> Data.ItemCount;
				stream >> Data.ItemName;
				stream >> Data.ItemType;
				stream >> Data.ItemExplan;

				ketcMakeItemInfo.MatInfo[j] = Data;
			}
			info.CB_SecondItems[i] = ketcMakeItemInfo;
		}
		return stream;
	}
};

class Packet_XMLEQItem
{
public:
	std::map<int, Equipment_ItemInfo> Equipment_Items;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLEQItem& info)
	{
		stream << info.Equipment_Items.size() << std::endl;
		for (auto& itetcItem : info.Equipment_Items)
		{
			stream << itetcItem.second.ItemID << std::endl;
			stream << itetcItem.second.OccupationID << std::endl;
			stream << itetcItem.second.ItemType << std::endl;
			stream << itetcItem.second.EquipmentType << std::endl;
			stream << itetcItem.second.ItemName << std::endl;
			stream << itetcItem.second.ItemExplan << std::endl;
		}
		return stream;
	}
	friend std::istream& operator>>(std::istream& stream, Packet_XMLEQItem& info)
	{
		int MapSize;
		Equipment_ItemInfo kEq_ItemInfo;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			stream >> kEq_ItemInfo.ItemID;
			stream >> kEq_ItemInfo.OccupationID;
			stream >> kEq_ItemInfo.ItemType;
			stream >> kEq_ItemInfo.EquipmentType;
			stream >> kEq_ItemInfo.ItemName;
			stream >> kEq_ItemInfo.ItemExplan;

			info.Equipment_Items[i] = kEq_ItemInfo;
		}

		return stream;
	}
};

// 수정 필요
class Packet_XMLLevel
{
public:
	std::map<int, Level_Info> Levels;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLLevel& info)
	{

		stream << info.Levels.size() << std::endl;

		for (auto& itLevel : info.Levels)
		{
			stream << itLevel.second.Level_ID << std::endl;
			stream << itLevel.second.Level_Name << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_XMLLevel& info)
	{
		int MapSize = 0;
		Level_Info kLevel_Info;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			stream >> kLevel_Info.Level_ID;
			stream >> kLevel_Info.Level_Name;

			info.Levels[i] = kLevel_Info;
		}

		return stream;
	}
};

class Packet_XMLShopInfo
{
public:
	std::map<int, Shop_ItemInfo> Shop_Items;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_XMLShopInfo& info)
	{
		stream << info.Shop_Items.size() << std::endl;
		for (auto& itItem : info.Shop_Items)
		{
			stream << itItem.second.BuyItemID << std::endl;
			stream << itItem.second.NeedItemID << std::endl;
			stream << itItem.second.NeedItemCount << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_XMLShopInfo& info)
	{
		int MapSize = 0;

		Shop_ItemInfo kShop_ItemInfo;

		stream >> MapSize;
		for (int i = 0; i < MapSize; i++)
		{
			stream >> kShop_ItemInfo.BuyItemID;
			stream >> kShop_ItemInfo.NeedItemID;
			stream >> kShop_ItemInfo.NeedItemCount;

			info.Shop_Items[i] = kShop_ItemInfo;
		}

		return stream;
	}
};

class Packet_CS_SignUp
{
public:
	std::string m_strID;
	std::string m_strPW;
	std::string m_strName;
	int m_nOccupation;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_SignUp& info)
	{
		stream << info.m_strID << std::endl;
		stream << info.m_strPW << std::endl;
		stream << info.m_strName << std::endl;
		stream << info.m_nOccupation << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_SignUp& info)
	{
		stream >> info.m_strID;
		stream >> info.m_strPW;
		stream >> info.m_strName;
		stream >> info.m_nOccupation;

		return stream;
	}
};

class Packet_Login
{
public:
	bool LoginResult;
	std::string UserCName;
	std::string Level;
	std::string Level_Name;
	int SessionID;
	int OccupationID;
	int LV;
	int MaxExp;
	int Experence;
	int MaxFatigue;
	int Fatigue;

	friend std::ostream& operator<<(std::ostream& stream, Packet_Login& info)
	{
		stream << info.LoginResult << std::endl;
		if (info.LoginResult)
		{
			stream << info.UserCName << std::endl;
			stream << info.Level << std::endl;
			stream << info.Level_Name << std::endl;
			stream << info.SessionID << std::endl;
			stream << info.OccupationID << std::endl;
			stream << info.LV << std::endl;
			stream << info.MaxExp << std::endl;
			stream << info.Experence << std::endl;
			stream << info.MaxFatigue << std::endl;
			stream << info.Fatigue << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_Login& info)
	{
		stream >> info.LoginResult;
		if (info.LoginResult)
		{
			stream >> info.UserCName;
			stream >> info.Level;
			stream >> info.Level_Name;
			stream >> info.SessionID;
			stream >> info.OccupationID;
			stream >> info.LV;
			stream >> info.MaxExp;
			stream >> info.Experence;
			stream >> info.MaxFatigue;
			stream >> info.Fatigue;
		}

		return stream;
	}
};

class Packet_Ingame
{
public:
	bool Result;
	int Level_ID;
	int InventoryMax;
	std::map<int, ItemInfo> Inventroys;
	std::map<int, ItemInfo> Equipments;

private:
	friend std::ostream& operator<<(std::ostream &stream, Packet_Ingame& info)
	{
		stream << info.Result << endl;

		if (info.Result)
		{
			stream << info.Level_ID << endl;
			stream << info.InventoryMax << endl;

			stream << info.Inventroys.size() << std::endl;
			for (auto& itInventory : info.Inventroys)
			{
				stream << itInventory.second.ItemID << std::endl;
				stream << itInventory.second.ItemCount << std::endl;
				stream << itInventory.second.SlotNumber << std::endl;
			}

			stream << info.Equipments.size() << std::endl;
			for (auto& itEquipment : info.Equipments)
			{
				stream << itEquipment.second.ItemID << std::endl;
				stream << itEquipment.second.SlotNumber << std::endl;
			}
		}
		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, Packet_Ingame& info)
	{
		int nInventoryCount = 0;
		int nEquipmentCount = 0;
		mat_ItemInfo Data;

		stream >> info.Result;

		if (info.Result)
		{
			stream >> info.Level_ID;
			stream >> info.InventoryMax;

			stream >> nInventoryCount;
			for (int i = 1; i <= nInventoryCount; i++)
			{
				ItemInfo kItemInfo;
				stream >> kItemInfo.ItemID;
				stream >> kItemInfo.ItemCount;
				stream >> kItemInfo.SlotNumber;

				info.Inventroys[i] = kItemInfo;
			}

			stream >> nEquipmentCount;
			for (int i = 1; i <= nEquipmentCount; i++)
			{
				ItemInfo kItemInfo;
				stream >> kItemInfo.ItemID;
				stream >> kItemInfo.SlotNumber;

				kItemInfo.ItemCount = 0;

				info.Equipments[i] = kItemInfo;
			}
		}
		return stream;
	}
};

class Packet_CS_LevelChange 
{
public:
	int SessionID;
	int BefroeLevel_ID;
	int AfterLevel_ID;

	friend std::ostream& operator<<(std::ostream &stream, Packet_CS_LevelChange& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.BefroeLevel_ID << std::endl;
		stream << info.AfterLevel_ID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_LevelChange& info)
	{
		stream >> info.SessionID;
		stream >> info.BefroeLevel_ID;
		stream >> info.AfterLevel_ID;

		return stream;
	}
};

class Packet_SC_LevelInfo
{
public:
	int SessionID;
	std::string Level_Name;

	friend std::ostream& operator<<(std::ostream &stream, Packet_SC_LevelInfo& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Level_Name << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_LevelInfo& info)
	{
		stream >> info.SessionID;
		stream >> info.Level_Name;

		return stream;
	}
};

class Packet_SC_UpdateLevel
{
public:
	int SessionID;
	std::string Level_Name;
	std::string Level_Name_ID;

	friend std::ostream& operator<<(std::ostream &stream, Packet_SC_UpdateLevel& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Level_Name << std::endl;
		stream << info.Level_Name_ID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_UpdateLevel& info)
	{
		stream >> info.SessionID;
		stream >> info.Level_Name;
		stream >> info.Level_Name_ID;

		return stream;
	}
};

class Packet_CS_WorldChat
{
public:
	std::string SendUserName;
	std::string Message;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_WorldChat& info)
	{
		stream << info.SendUserName << std::endl;
		stream << info.Message << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_WorldChat& info)
	{
		stream >> info.SendUserName;
		std::string msg;

		getline(stream, msg);
		while (stream >> msg)
		{
			info.Message += msg + "_";
		}

		return stream;
	}
};

class Packet_CS_LevelChat
{
public:
	int SendUserID;
	std::string SendUserName;
	std::string Message;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_LevelChat& info)
	{
		stream << info.SendUserID << std::endl;
		stream << info.SendUserName << std::endl;
		stream << info.Message << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_LevelChat& info)
	{
		stream >> info.SendUserID;
		stream >> info.SendUserName;
		std::string msg;

		getline(stream, msg);
		while (stream >> msg)
		{
			info.Message += msg + "_";
		}

		return stream;
	}
};

class Packet_CS_WhisperChat
{
public:
	int SendUserID;
	std::string SendUserName;
	int RecvUserID;
	std::string Message;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_WhisperChat& info)
	{
		stream << info.SendUserID << std::endl;
		stream << info.SendUserName << std::endl;
		stream << info.RecvUserID << std::endl;
		stream << info.Message << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_WhisperChat& info)
	{
		stream >> info.SendUserID;
		stream >> info.SendUserName;
		stream >> info.RecvUserID;

		std::string msg;
		getline(stream, msg);
		while (stream >> msg)
		{
			info.Message += msg + "_";
		}

		return stream;
	}
};

class Packet_SC_Message
{
public:
	int ChatType;
	std::string SendUserName;
	std::string Message;

	friend std::ostream& operator<<(std::ostream &stream, Packet_SC_Message& info)
	{
		stream << info.ChatType << std::endl;
		stream << info.SendUserName << std::endl;
		stream << info.Message << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_Message& info)
	{
		stream >> info.ChatType;
		stream >> info.SendUserName;
		stream >> info.Message;

		return stream;
	}
};

class Packet_CS_TryGather
{
public:
	int SessionID;
	int Occupation;
	int Interaction_ID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_TryGather& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Occupation << std::endl;
		stream << info.Interaction_ID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_TryGather& info)
	{
		stream >> info.SessionID;
		stream >> info.Occupation;
		stream >> info.Interaction_ID;
		

		return stream;
	}
};

class Packet_SC_TryGather
{
public:
	bool result;
	int SessionID;
	int Interaction_ID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_TryGather& info)
	{
		stream << info.result << std::endl;
		if (info.result)
		{
			stream << info.SessionID << std::endl;
			stream << info.Interaction_ID << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_TryGather& info)
	{
		stream >> info.result;

		if (info.result)
		{
			stream >> info.SessionID;
			stream >> info.Interaction_ID;
		}

		return stream;
	}
};

class Packet_CS_CancelGather
{
public:
	int SessionID;
	int Interaction_ID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_CancelGather& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Interaction_ID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_CancelGather& info)
	{

		stream >> info.SessionID;
		stream >> info.Interaction_ID;
		

		return stream;
	}
};

class Packet_SC_CancelGather
{
public:
	int SessionID;
	int Interaction_ID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_CancelGather& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Interaction_ID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_CancelGather& info)
	{

		stream >> info.SessionID;
		stream >> info.Interaction_ID;


		return stream;
	}
};

class Packet_Combination
{
public:
	bool Result;
	// 얻는 아이템 ID
	int Item_ID;
	int ItemCount;
	int SlotNum;
	// 조합 후 남은 아이템;
	std::map<int, ItemInfo> UpdateHasItems;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_Combination& info)
	{
		stream << info.Result << std::endl;
		if (info.Result)
		{
			stream << info.Item_ID << std::endl;
			stream << info.ItemCount << std::endl;
			stream << info.SlotNum << std::endl;

			stream << info.UpdateHasItems.size() << std::endl;
			for (auto& ititem : info.UpdateHasItems)
			{
				stream << ititem.second.ItemID << std::endl;
				stream << ititem.second.ItemCount << std::endl;
				stream << ititem.second.SlotNumber << std::endl;
			}
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_Combination& info)
	{
		int nItmesCount = 0;

		stream >> info.Result;

		if (info.Result)
		{
			stream >> info.Item_ID;
			stream >> info.ItemCount;
			stream >> info.SlotNum;

			stream >> nItmesCount;

			for (int i = 0; i < nItmesCount; i++)
			{
				ItemInfo kItemInfo;
				stream >> kItemInfo.ItemID;
				stream >> kItemInfo.ItemCount;
				stream >> kItemInfo.SlotNumber;

				info.UpdateHasItems[i] = kItemInfo;
			}
		}

		return stream;
	}
};

class Packet_CS_ItemUse
{
public:
	int SessionID;
	int ItemID;
	int SlotNumber;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_ItemUse& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.ItemID << std::endl;
		stream << info.SlotNumber << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_ItemUse& info)
	{
		stream >> info.SessionID;
		stream >> info.ItemID;
		stream >> info.SlotNumber;

		return stream;
	}
};

class Packet_SC_ItemUse
{
public:
	bool Result;
	int SessionID;
	int ItemCount;
	int SlotNumber;
	int User_Fatigue;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_ItemUse& info)
	{
		stream << info.Result;
		if (info.Result)
		{
			stream << info.SessionID << std::endl;
			stream << info.ItemCount << std::endl;
			stream << info.SlotNumber << std::endl;
			stream << info.User_Fatigue << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_ItemUse& info)
	{
		stream >> info.Result;
		if (info.Result)
		{
			stream >> info.SessionID;
			stream >> info.ItemCount;
			stream >> info.SlotNumber;
			stream >> info.User_Fatigue;
		}
		return stream;
	}
};

class Packet_CS_ThrowItem
{
public:
	int SessionID;
	int ItemType;
	int SlotNumber;
	int ItemCount;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_ThrowItem& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.ItemType << std::endl;
		stream << info.SlotNumber << std::endl;
		stream << info.ItemCount << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_ThrowItem& info)
	{
		stream >> info.SessionID;
		stream >> info.ItemType;
		stream >> info.SlotNumber;
		stream >> info.ItemCount;

		return stream;
	}
};

class Packet_SC_ThrowITem
{
public:
	bool Result;
	int ItemType;
	int SlotNumber;
	int ItemCount;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_ThrowITem& info)
	{
		stream << info.Result << std::endl;
		if (info.Result)
		{
			stream << info.ItemType << std::endl;
			stream << info.SlotNumber << std::endl;
			stream << info.ItemCount << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_ThrowITem& info)
	{
		stream >> info.Result;

		if (info.Result)
		{
			stream >> info.ItemType;
			stream >> info.SlotNumber;
			stream >> info.ItemCount;
		}

		return stream;
	}
};

class Packet_CS_MountEquipment
{
public:
	int SessionID;
	// 장착하려는 아이템 타입
	int Equipment_Type;
	// 장착하려는 아이템의 인벤토리 슬롯 정보
	int SlotNumber;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_MountEquipment& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Equipment_Type << std::endl;
		stream << info.SlotNumber << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_MountEquipment& info)
	{
		stream >> info.SessionID;
		stream >> info.Equipment_Type;
		stream >> info.SlotNumber;

		return stream;
	}
};

class Packet_SC_MountEquipment
{
public:
	bool Result;

	int Equipment_SlotType = -1;
	int Inventory_SlotNumber = -1;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_MountEquipment& info)
	{
		stream << info.Result << std::endl;
		if (info.Result)
		{
			stream << info.Equipment_SlotType << std::endl;
			stream << info.Inventory_SlotNumber << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_MountEquipment& info)
	{
		stream >> info.Result;

		if (info.Result)
		{
			stream >> info.Equipment_SlotType;
			stream >> info.Inventory_SlotNumber;
		}

		return stream;
	}
};

class Packet_CS_UnMountEquipment
{
public:
	int SessionID;
	// 장착 해제하려는 장비창 슬롯 정보
	int SlotNumber;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_UnMountEquipment& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.SlotNumber << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_UnMountEquipment& info)
	{
		stream >> info.SessionID;
		stream >> info.SlotNumber;

		return stream;
	}
};

class Packet_SC_UnMountEquipment
{
public:
	bool Result;

	// 해체한 장비창 슬롯 정보
	int Equipment_SlotType;
	// 인벤토리로 돌아갈 슬롯 정보
	int Inventory_SlotNumber;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_UnMountEquipment& info)
	{
		stream << info.Result << std::endl;
		if (info.Result)
		{
			stream << info.Equipment_SlotType << std::endl;
			stream << info.Inventory_SlotNumber << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_UnMountEquipment& info)
	{
		stream >> info.Result;

		if (info.Result)
		{
			stream >> info.Equipment_SlotType;
			stream >> info.Inventory_SlotNumber;
		}

		return stream;
	}
};

class Packet_CS_BuyShopItem
{
public:
	int SessionID;
	int BuyItemID;
	int BuyItemCount;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_BuyShopItem& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.BuyItemID << std::endl;
		stream << info.BuyItemCount << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_BuyShopItem& info)
	{
		stream >> info.SessionID;
		stream >> info.BuyItemID;
		stream >> info.BuyItemCount;

		return stream;
	}
};

class Packet_SC_BuyShopItem
{
public:
	bool Result;
	int GetItemID;
	int GetItemCount;
	int GetItemSlot;

	int ConsumeItemID;
	int ConsumeItemCount;
	int ConsumeItemSlot;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_BuyShopItem& info)
	{
		stream << info.Result << std::endl;
		if (info.Result)
		{
			stream << info.GetItemID << std::endl;
			stream << info.GetItemCount << std::endl;
			stream << info.GetItemSlot << std::endl;

			stream << info.ConsumeItemID << std::endl;
			stream << info.ConsumeItemCount << std::endl;
			stream << info.ConsumeItemSlot << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_BuyShopItem& info)
	{
		stream >> info.Result;
		if (info.Result)
		{
			stream >> info.GetItemID;
			stream >> info.GetItemCount;
			stream >> info.GetItemSlot;

			stream >> info.ConsumeItemID;
			stream >> info.ConsumeItemCount;
			stream >> info.ConsumeItemSlot;
		}

		return stream;
	}
};