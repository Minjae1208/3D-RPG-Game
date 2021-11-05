#include "DataShop.h"

bool DataShops::parse(std::string url)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataShop* shop;
	int m_nOccupation = -1;


	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		shop = new DataShop();
		m_nOccupation = m_elem->IntAttribute("Occupation");
		shop->SetBuyItemID(m_elem->IntAttribute("Buy_Item"));
		shop->SetNeedItemID(m_elem->IntAttribute("Need_Item"));
		shop->SetNeedItemCount(m_elem->IntAttribute("Need_Count"));

		switch (m_nOccupation)
		{
		case EOccupation::GATHERER:	GathererShop_Data[shop->GetBuyItemID()] = shop;		break;
		case EOccupation::MINOR:	MinorShop_Data[shop->GetBuyItemID()] = shop;		break;
		case EOccupation::HUNTER:	HunterShop_Data[shop->GetBuyItemID()] = shop;		break;
		}
	}

	return true;
}

void DataShop::GetAllData(Shop_ItemInfo * kShop_ItemInfo)
{
	kShop_ItemInfo->BuyItemID = BuyItemID;
	kShop_ItemInfo->NeedItemID = NeedItemID;
	kShop_ItemInfo->NeedItemCount = NeedItemCount;
}
