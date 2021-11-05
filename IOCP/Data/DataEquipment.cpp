#include "DataEquipment.h"

bool DataEquipments::parse(std::string url)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataEquipment* EquipmentItem;

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		EquipmentItem = new DataEquipment();

		EquipmentItem->SetID(m_elem->IntAttribute("ID"));
		EquipmentItem->SetType(m_elem->IntAttribute("Item_Type"));
		EquipmentItem->SetEQType(m_elem->IntAttribute("Equipment_Type"));
		EquipmentItem->SetOccupation(m_elem->IntAttribute("Occupation"));
		EquipmentItem->SetName(m_elem->Attribute("Name"));
		EquipmentItem->SetExplan(m_elem->Attribute("Explan"));
		EquipmentItem->SetStat(m_elem->IntAttribute("Stat"));

		Equipment_Data[EquipmentItem->GetID()] = EquipmentItem;
	}

	return true;
}

void DataEquipment::GetAllData(Equipment_ItemInfo * kEquipment_ItemInfo)
{
	kEquipment_ItemInfo->ItemID = ID;
	kEquipment_ItemInfo->OccupationID = Occupation;
	kEquipment_ItemInfo->ItemType = Type;
	kEquipment_ItemInfo->EquipmentType = EQType;
	kEquipment_ItemInfo->ItemName = Name;
	kEquipment_ItemInfo->ItemExplan = Explan;
	kEquipment_ItemInfo->Stat = Stat;
}
