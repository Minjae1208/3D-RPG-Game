#pragma once

#include <map>
#include <iostream>
#include "../Struct/stLevel.h"

// 채집물 정보
class PacketInteractionInfo
{
public:
	std::map<int, Interaction> Interactions;

private:
	friend std::ostream& operator<<(std::ostream &stream, PacketInteractionInfo& info)
	{
		stream << info.Interactions.size() << std::endl;
		for (auto& itInteraction : info.Interactions)
		{
			stream << itInteraction.second.Name << std::endl;
			stream << itInteraction.second.ID << std::endl;
			stream << itInteraction.second.Type_ID << std::endl;
			stream << itInteraction.second.Drop_ID << std::endl;
			stream << itInteraction.second.State << std::endl;
			stream << itInteraction.second.X << std::endl;
			stream << itInteraction.second.Y << std::endl;
			stream << itInteraction.second.Z << std::endl;
			stream << itInteraction.second.Experience << std::endl;
			stream << itInteraction.second.Fatigue << std::endl;
		}
		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, PacketInteractionInfo& info)
	{
		int nMonsterCount = 0;
		stream >> nMonsterCount;
		for (int i = 0; i < nMonsterCount; i++)
		{
			Interaction kInteraction;

			stream >> kInteraction.Name;
			stream >> kInteraction.ID;
			stream >> kInteraction.Type_ID;
			stream >> kInteraction.Drop_ID;
			stream >> kInteraction.State;
			stream >> kInteraction.X;
			stream >> kInteraction.Y;
			stream >> kInteraction.Z;
			stream >> kInteraction.Experience;
			stream >> kInteraction.Fatigue;

			info.Interactions[i] = kInteraction;
		}
		return stream;
	}
};

// 동물 움직임 정보
class Packet_CS_InteractionMove
{
public:
	Interaction kInteraction;

private:
	friend std::ostream& operator<<(std::ostream &stream, Packet_CS_InteractionMove& info)
	{
		stream << info.kInteraction.Name << std::endl;
		stream << info.kInteraction.ID << std::endl;
		stream << info.kInteraction.Type_ID << std::endl;
		stream << info.kInteraction.Drop_ID << std::endl;
		stream << info.kInteraction.State << std::endl;
		stream << info.kInteraction.X << std::endl;
		stream << info.kInteraction.Y << std::endl;
		stream << info.kInteraction.Z << std::endl;
		stream << info.kInteraction.Experience << std::endl;
		stream << info.kInteraction.Fatigue << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, Packet_CS_InteractionMove& info)
	{
		stream >> info.kInteraction.Name;
		stream >> info.kInteraction.ID;
		stream >> info.kInteraction.Type_ID;
		stream >> info.kInteraction.Drop_ID;
		stream >> info.kInteraction.State;
		stream >> info.kInteraction.X;
		stream >> info.kInteraction.Y;
		stream >> info.kInteraction.Z;
		stream >> info.kInteraction.Experience;
		stream >> info.kInteraction.Fatigue;

		return stream;
	}
};

// 채집물 리스폰
class Packet_SC_RespawnInteraction
{
public:
	Interaction kInteraction;

private:
	friend std::ostream& operator<<(std::ostream &stream, Packet_SC_RespawnInteraction& info)
	{
		stream << info.kInteraction.Name << std::endl;
		stream << info.kInteraction.ID << std::endl;
		stream << info.kInteraction.Type_ID << std::endl;
		stream << info.kInteraction.Drop_ID << std::endl;
		stream << info.kInteraction.State << std::endl;
		stream << info.kInteraction.X << std::endl;
		stream << info.kInteraction.Y << std::endl;
		stream << info.kInteraction.Z << std::endl;
		stream << info.kInteraction.Experience << std::endl;
		stream << info.kInteraction.Fatigue << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, Packet_SC_RespawnInteraction& info)
	{
		stream >> info.kInteraction.Name;
		stream >> info.kInteraction.ID;
		stream >> info.kInteraction.Type_ID;
		stream >> info.kInteraction.Drop_ID;
		stream >> info.kInteraction.State;
		stream >> info.kInteraction.X;
		stream >> info.kInteraction.Y;
		stream >> info.kInteraction.Z;
		stream >> info.kInteraction.Experience;
		stream >> info.kInteraction.Fatigue;

		return stream;
	}

};

// 채집 성공
class Packet_SC_SuccessGather
{
public:
	int SlotNumber;
	int DropItem_ID;
	int ItemCount;
	int LV;
	int MaxExp;
	int Experience;
	int MaxFatigue;
	int Fatigue;

private:
	friend std::ostream& operator<<(std::ostream &stream, Packet_SC_SuccessGather& info)
	{
		stream << info.SlotNumber << std::endl;
		stream << info.DropItem_ID << std::endl;
		stream << info.ItemCount << std::endl;
		stream << info.LV << std::endl;
		stream << info.MaxExp << std::endl;
		stream << info.Experience << std::endl;
		stream << info.MaxFatigue << std::endl;
		stream << info.Fatigue << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, Packet_SC_SuccessGather& info)
	{
		stream >> info.SlotNumber;
		stream >> info.DropItem_ID;
		stream >> info.ItemCount;
		stream >> info.LV;
		stream >> info.MaxExp;
		stream >> info.Experience;
		stream >> info.MaxFatigue;
		stream >> info.Fatigue;

		return stream;
	}

};


class PacketEnter
{
public:
	int SessionID;
	std::string Name;
	int OccupationID;
	float X;
	float Y;
	float Z;

private:
	friend std::ostream& operator<<(std::ostream &stream, PacketEnter& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.Name << std::endl;
		stream << info.OccupationID << std::endl;
		stream << info.X << std::endl;
		stream << info.Y << std::endl;
		stream << info.Z << std::endl;

		return stream;
	}

	friend std::istream& operator>>(std::istream &stream, PacketEnter& info)
	{
		stream >> info.SessionID;
		stream >> info.Name;
		stream >> info.OccupationID;
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;

		return stream;
	}
};

class PacketEnterInfo
{
public:

	std::map<int, PacketEnter> players;

private:
	friend std::ostream& operator<<(std::ostream &stream, PacketEnterInfo& info)
	{
		stream << info.players.size() << std::endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << std::endl;
			stream << kvp.second << std::endl;
		}

		return stream;
	}

	friend std::istream &operator>>(std::istream &stream, PacketEnterInfo& info)
	{
		int nPlayers = 0;
		int SessionID = 0;
		PacketEnter Player;
		info.players.clear();

		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++)
		{
			stream >> SessionID;
			stream >> Player;
			info.players[SessionID] = Player;
		}

		return stream;
	}
};