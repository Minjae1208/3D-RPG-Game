#include "DataInteraction.h"

bool DataInteraction::parse(std::string url, DataLevel & pDataLevel)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		m_elem = m_xmlData->ToElement();

		Interaction* pInteraction = new Interaction();
		pInteraction->ID = m_elem->IntAttribute("ID");
		pInteraction->TimerID = m_elem->IntAttribute("Timer");
		pInteraction->Type_ID = m_elem->IntAttribute("Type");
		pInteraction->Name = m_elem->Attribute("Name");
		pInteraction->Drop_ID = m_elem->IntAttribute("Drop");
		pInteraction->State = 1;
		pInteraction->X = m_elem->FloatAttribute("X");
		pInteraction->Y = m_elem->FloatAttribute("Y");
		pInteraction->Z = m_elem->FloatAttribute("Z");
		pInteraction->Experience = m_elem->IntAttribute("Experience");
		pInteraction->Fatigue = m_elem->IntAttribute("Fatigue");

		pDataLevel.Interactions[pInteraction->ID] = pInteraction;
	}
	return true;
}