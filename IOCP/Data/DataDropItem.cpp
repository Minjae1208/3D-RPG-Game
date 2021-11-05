#include "DataDropItem.h"

bool DataDropItems::parse(std::string url)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataDropItem* dropitem;
	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		dropitem = new DataDropItem();

		dropitem->SetID(m_elem->IntAttribute("ID"));
		dropitem->SetType(m_elem->IntAttribute("Type"));
		dropitem->SetName(m_elem->Attribute("Name"));
		dropitem->SetExplan(m_elem->Attribute("Explan"));

		DropItem_Data[dropitem->GetID()] = dropitem;
	}
	return true;
}

void DataDropItem::GetAllData(Drop_ItemInfo * kDrop_ItemInfo)
{
	kDrop_ItemInfo->ItemID = ID;
	kDrop_ItemInfo->ItemType = Type;
	kDrop_ItemInfo->ItemName = Name;
	kDrop_ItemInfo->ItemExplan = Explan;
}
