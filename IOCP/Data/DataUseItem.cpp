#include "DataUseItem.h"

bool DataUseItems::parse(std::string url)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataUseItem* UseItem;

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		UseItem = new DataUseItem();

		UseItem->SetID(m_elem->IntAttribute("ID"));
		UseItem->SetFatigue(m_elem->IntAttribute("Fatigue"));

		UseItem_Data[UseItem->GetID()] = UseItem;
	}
	return true;
}
