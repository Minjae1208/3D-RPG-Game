#include "Data.h"


enum EParse 
{
	LEVEL,
	CHARACTER,
	DROPITEM,
	USEITEM,
	CBFIRST,
	CBSECOND,
	EQUIPMENT,
	SHOP,
	END,
};

bool Data::Parse(std::map<int, Level*> & Levels, std::map<int, DataLevel*> & DataLevels)
{
	int index = EParse::LEVEL;

	while (true)
	{
		switch (index)
		{
		case EParse::LEVEL:				if(Maps->parse("XML/Map.xml", Levels, DataLevels) == false)				parse_error(index); break;
		case EParse::CHARACTER:			if(characters->parse("XML/Character.xml") == false)						parse_error(index); break;
		case EParse::DROPITEM:			if(DropItems->parse("XML/Drop_Item.xml") == false)						parse_error(index); break;
		case EParse::USEITEM:			if(UseItems->parse("XML/Use_Item.xml") == false)						parse_error(index); break;
		case EParse::CBFIRST:			if(CBFirsts->parse("XML/CBFirst.xml", DropItems) == false)				parse_error(index); break;
		case EParse::CBSECOND:			if(CBSeconds->parse("XML/CBSecond.xml", CBFirsts) == false)				parse_error(index); break;
		case EParse::EQUIPMENT:			if(Equipments->parse("XML/Equipment_Item.xml") == false)				parse_error(index); break;
		case EParse::SHOP:				if(Shops->parse("XML/Shop.xml") == false)								parse_error(index); break;
		case EParse::END:				return true;
		}

		index++;
	}

	return false;
}

bool Data::parse_error(int nindex)
{
	printf_s("[INFO][InGameServer] [index : %d] XML Parsing Error!", nindex);

	return false;
}
