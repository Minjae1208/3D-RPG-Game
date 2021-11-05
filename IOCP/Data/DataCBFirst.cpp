#include "DataCBFirst.h"

bool DataCBFirsts::parse(std::string url, DataDropItems * pDropItem)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataCBFirst* CBFirst;

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		CBFirst = new DataCBFirst();

		CBFirst->SetID(m_elem->IntAttribute("ID"));
		CBFirst->SetType(m_elem->IntAttribute("Type"));
		CBFirst->SetName(m_elem->Attribute("Name"));
		CBFirst->SetExplan(m_elem->Attribute("Explan"));

		DataCBMat Mat;

		std::string strsub1;
		std::string strsub2;
		int m_nCount = 1;

		while (true)
		{
			strsub1 = "CB" + std::to_string(m_nCount) + "_ID";
			strsub2 = "CB" + std::to_string(m_nCount) + "_Count";

			int SubID = m_elem->IntAttribute(strsub1.c_str());
			int SubCount = m_elem->IntAttribute(strsub2.c_str());

			if (SubID == 0 || SubCount == 0)
				break;

			Mat.SetID(SubID);
			Mat.SetCount(SubCount);
			Mat.SetType(pDropItem->DropItem_Data[SubID]->GetType());
			Mat.SetName(pDropItem->DropItem_Data[SubID]->GetName());
			Mat.SetExplan(pDropItem->DropItem_Data[SubID]->GetExplan());

			CBFirst->SetMatData(m_nCount, Mat);

			m_nCount++;
		}

		CBFirst_Data[CBFirst->GetID()] = CBFirst;
	}

	return true;
}

void DataCBFirst::GetAllData(CB_ItemInfo * kCB_ItemInfo)
{
	kCB_ItemInfo->ItemID = ID;
	kCB_ItemInfo->ItemType = Type;
	kCB_ItemInfo->ItemName = Name;
	kCB_ItemInfo->ItemExplan = Explan;

	mat_ItemInfo * kmat_ItemInfo;

	int m_nsize = mat_Data.size();
	for (int i = 0; i < m_nsize; i++)
	{
		kmat_ItemInfo = new mat_ItemInfo;
		DataCBMat cDataCBMat = mat_Data[i + 1];
		cDataCBMat.GetAllData(kmat_ItemInfo);
		kCB_ItemInfo->MatInfo[i + 1] = *kmat_ItemInfo;
	}
}
