#include "DataMap.h"

bool DataMaps::parse(std::string url, std::map<int, Level*> & Levels, std::map<int, DataLevel*> & DataLevels)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataLevel* pDataLevel;
	Level* pLevel;
	DataMap* map;

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		pDataLevel = new DataLevel();
		map = new DataMap();

		map->ID = m_elem->IntAttribute("ID");
		map->NameID = m_elem->Attribute("Name_ID");
		map->Name = m_elem->Attribute("Name");
		map->Type = m_elem->IntAttribute("Type");
		map->Interaction = m_elem->IntAttribute("Interaction");

		if (map->Interaction == 0) // 몬스터가 없는 경우
		{
			pLevel = new HomeLevel();
		}
		else // 몬스터가 있는 경우
		{
			pLevel = new InteractionLevel();
			std::string m_strName = map->NameID;
			m_strName = "XML/" + m_strName + ".xml";
			if (!Interactions->parse(m_strName, *pDataLevel))
			{
				return false;
			}
		}

		DataLevels[map->ID] = pDataLevel;
		pLevel->Init(DataLevels[map->ID]);
		Levels[map->ID] = pLevel;
		Levels[map->ID]->Level_ID = map->ID;
		Map_Data[map->ID] = map;
	}

	return true;
}