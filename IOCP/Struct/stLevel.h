#pragma once

#include <string>

enum EInteractionType {
	FLOWER = 1,
	TREE,
	ANIMAL,
};

struct Interaction {
	std::string Name;
	int ID;		// ä���� ID
	int TimerID;
	int Type_ID;				// Ÿ�� ID (�Ĺ�, ����, ����)
	int Drop_ID;				// ��� ������ ID
	// ���°� (�����Ǿ��ִ� ���� = 1, ĳ���� ���� = 2, ���� ��� ���� = 3)
	int State;

	int Experience;
	int Fatigue;
	// int get_Experience;
	// int consume_Fatigue;

	float X;
	float Y;
	float Z;
};

struct DataLevel {
	int x_max;
	int x_min;
	int y_max;
	int y_min;

	std::map<int, Interaction*> Interactions;

	~DataLevel()
	{
		for (auto Interaction : Interactions)
		{
			delete Interaction.second;
		}
		Interactions.empty();
	}
};


struct Level_Info {
	int Level_ID;
	std::string Level_Name;
};