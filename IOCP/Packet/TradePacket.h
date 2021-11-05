#pragma once

#include <sstream>
#include <map>
#include "../Struct/stUser.h"
#include "../Struct/stTrade.h"

class Packet_Send_TryTrade
{
public:
	// 보낸 사람 정보
	int SendSessionID;
	std::string SendNickName;

	// 받는 사람 정보
	int RecvSessionID;
	std::string RecvNickName;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_Send_TryTrade& info)
	{
		stream << info.SendSessionID << std::endl;
		stream << info.SendNickName << std::endl;

		stream << info.RecvSessionID << std::endl;
		stream << info.RecvNickName << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_Send_TryTrade& info)
	{
		stream >> info.SendSessionID;
		stream >> info.SendNickName;

		stream >> info.RecvSessionID;
		stream >> info.RecvNickName;

		return stream;
	}
};

class Packet_Recv_TryTrade
{
public:
	bool Result;
	int SendSessionID;
	int RecvSessionID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_Recv_TryTrade& info)
	{
		stream << info.Result << std::endl;
		stream << info.SendSessionID << std::endl;
		stream << info.RecvSessionID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_Recv_TryTrade& info)
	{
		stream >> info.Result;
		stream >> info.SendSessionID;
		stream >> info.RecvSessionID;

		return stream;
	}
};

class Packet_SC_TradeMaching
{
public:
	bool Result;
	int TradeID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_TradeMaching& info)
	{
		stream << info.Result << std::endl;
		if (info.Result == true)
			stream << info.TradeID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_TradeMaching& info)
	{
		stream >> info.Result;
		if (info.Result == true)
			stream >> info.TradeID;

		return stream;
	}
};

class Packet_SC_PutOnItem
{
public:
	int TradeID;
	int SessionID;
	int itemID;
	int ItemCount;
	int InventorySlot;
	int TradSlot = 0;
	int ItemType;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_PutOnItem& info)
	{
		stream << info.TradeID << std::endl;
		stream << info.SessionID << std::endl;
		stream << info.itemID << std::endl;
		stream << info.ItemCount << std::endl;
		stream << info.InventorySlot << std::endl;
		stream << info.TradSlot << std::endl;
		stream << info.ItemType << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_PutOnItem& info)
	{
		stream >> info.TradeID;
		stream >> info.SessionID;
		stream >> info.itemID;
		stream >> info.ItemCount;
		stream >> info.InventorySlot;
		stream >> info.TradSlot;
		stream >> info.ItemType;

		return stream;
	}
};

class Packet_CS_TradeAccept
{
public:
	int TradeID;
	int SessionID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_TradeAccept& info)
	{
		stream << info.TradeID << std::endl;
		stream << info.SessionID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_TradeAccept& info)
	{
		stream >> info.TradeID;
		stream >> info.SessionID;

		return stream;
	}
};

class Packet_CS_TradeCancel
{
public:
	int TradeID;
	int SessionID;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_CS_TradeCancel& info)
	{
		stream << info.TradeID << std::endl;
		stream << info.SessionID << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_CS_TradeCancel& info)
	{
		stream >> info.TradeID;
		stream >> info.SessionID;

		return stream;
	}
};

class Packet_SC_TradeCancel
{
public:
	int SessionID;
	std::map<int, struct ItemInfo> InventoryInfo;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_TradeCancel& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.InventoryInfo.size() << std::endl;
		for (auto& itItemInfo : info.InventoryInfo)
		{
			stream << itItemInfo.second.ItemID << std::endl;
			stream << itItemInfo.second.ItemCount << std::endl;
			stream << itItemInfo.second.SlotNumber << std::endl;
			stream << itItemInfo.second.ItemType << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_TradeCancel& info)
	{
		int ItemCount;

		stream >> info.SessionID;
		stream >> ItemCount;

		for (int i = 0; i < ItemCount; i++)
		{
			ItemInfo kItemInfo;
			stream >> kItemInfo.ItemID;
			stream >> kItemInfo.ItemCount;
			stream >> kItemInfo.SlotNumber;
			stream >> kItemInfo.ItemType;

			info.InventoryInfo[kItemInfo.ItemID] = kItemInfo;
		}

		return stream;
	}
};

class Packet_SC_TradeCompletion
{
public:
	int SessionID;
	std::map<int, struct ItemInfo> InventoryInfo;

private:
	friend std::ostream& operator<<(std::ostream& stream, Packet_SC_TradeCompletion& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.InventoryInfo.size() << std::endl;
		for (auto& itItemInfo : info.InventoryInfo)
		{
			stream << itItemInfo.second.ItemID << std::endl;
			stream << itItemInfo.second.ItemCount << std::endl;
			stream << itItemInfo.second.SlotNumber << std::endl;
			stream << itItemInfo.second.ItemType << std::endl;
		}

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Packet_SC_TradeCompletion& info)
	{
		int ItemCount;

		stream >> info.SessionID;
		stream >> ItemCount;

		for (int i = 0; i < ItemCount; i++)
		{
			struct ItemInfo kItemInfo;
			stream >> kItemInfo.ItemID;
			stream >> kItemInfo.ItemCount;
			stream >> kItemInfo.SlotNumber;
			stream >> kItemInfo.ItemType;

			info.InventoryInfo[kItemInfo.ItemID] = kItemInfo;
		}

		return stream;
	}
};