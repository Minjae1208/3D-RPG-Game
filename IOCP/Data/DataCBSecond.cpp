#include "DataCBSecond.h"

bool DataCBSeconds::parse(std::string url, DataCBFirsts *  pCBFirst)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataCBSecond* CBSecond;

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		CBSecond = new DataCBSecond();

		CBSecond->SetID(m_elem->IntAttribute("ID"));
		CBSecond->SetType(m_elem->IntAttribute("Type"));
		CBSecond->SetName(m_elem->Attribute("Name"));
		CBSecond->SetExplan(m_elem->Attribute("Explan"));

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
			Mat.SetType(pCBFirst->CBFirst_Data[SubID]->GetType());
			Mat.SetName(pCBFirst->CBFirst_Data[SubID]->GetName());
			Mat.SetExplan(pCBFirst->CBFirst_Data[SubID]->GetExplan());

			CBSecond->SetMatData(m_nCount, Mat);

			m_nCount++;
		}

		CBSecond_Data[CBSecond->GetID()] = CBSecond;
	}

	return true;
}

void DataCBSecond::GetAllData(CB_ItemInfo * kCB_ItemInfo)
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
		kCB_ItemInfo->MatInfo[i+1] = *kmat_ItemInfo;
	}
}
