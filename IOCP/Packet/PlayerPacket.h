#pragma once

#include <map>
#include <iostream>

class Player {
public:
	int	SessionID;		// ���� ���̵�
	int OccupationID;	// ����
	int LV;				// ���� ����
	int Experience;		// ����ġ
	int Fatigue;		// �Ƿε�
	int Level_ID;		// ��

	float X;			// ��ġ
	float Y;
	float Z;
	
	float Yaw;			// ȸ����
	float Pitch;
	float Roll;

	friend std::ostream& operator<<(std::ostream &stream, Player& info)
	{
		stream << info.SessionID << std::endl;
		stream << info.OccupationID << std::endl;
		stream << info.LV << std::endl;
		stream << info.Experience << std::endl;
		stream << info.Fatigue << std::endl;
		stream << info.Level_ID << std::endl;
		stream << info.X << std::endl;
		stream << info.Y << std::endl;
		stream << info.Z << std::endl;
		stream << info.Yaw << std::endl;
		stream << info.Pitch << std::endl;
		stream << info.Roll << std::endl;
		

		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Player& info)
	{
		stream >> info.SessionID;
		stream >> info.OccupationID;
		stream >> info.LV;
		stream >> info.Experience;
		stream >> info.Fatigue;
		stream >> info.Level_ID;
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;
		stream >> info.Yaw;
		stream >> info.Pitch;
		stream >> info.Roll;

		return stream;
	}
};

class Players
{
public:
	std::map<int, Player> players;

	friend std::ostream& operator<<(std::ostream &stream, Players& info)
	{
		stream << info.players.size() << std::endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << std::endl;
			stream << kvp.second << std::endl;
		}

		return stream;
	}

	friend std::istream &operator>>(std::istream &stream, Players& info)
	{
		int nPlayers = 0;
		int SessionID = 0;
		Player player;
		info.players.clear();

		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++)
		{
			stream >> SessionID;
			stream >> player;
			info.players[SessionID] = player;
		}

		return stream;
	}
};