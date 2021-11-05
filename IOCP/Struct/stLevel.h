#pragma once

#include <string>

enum EInteractionType {
	FLOWER = 1,
	TREE,
	ANIMAL,
};

struct Interaction {
	std::string Name;
	int ID;		// 채집물 ID
	int TimerID;
	int Type_ID;				// 타입 ID (식물, 동물, 광물)
	int Drop_ID;				// 드롭 아이템 ID
	// 상태값 (스폰되어있는 상태 = 1, 캐지는 상태 = 2, 스폰 대기 상태 = 3)
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