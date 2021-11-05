#pragma once
#include "DataCharacter.h"
#include "DataMap.h"
#include "DataDropItem.h"
#include "DataUseItem.h"
#include "DataCBFirst.h"
#include "DataCBSecond.h"
#include "DataEquipment.h"
#include "DataShop.h"

class Data
{
public:
	DataCharacters* characters = new DataCharacters();
	DataMaps* Maps = new DataMaps();
	DataDropItems* DropItems = new DataDropItems();
	DataUseItems* UseItems = new DataUseItems();
	DataCBFirsts* CBFirsts = new DataCBFirsts();
	DataCBSeconds* CBSeconds = new DataCBSeconds();
	DataEquipments* Equipments = new DataEquipments();
	DataShops* Shops = new DataShops();

	bool Parse(std::map<int, Level*> & Levels, std::map<int, struct DataLevel*> & DataLevels);
	bool parse_error(int nindex);
};

