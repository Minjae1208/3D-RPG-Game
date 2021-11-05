#include "DataCharacter.h"

// XML µ¥ÀÌÅÍ ÆÄ½Ì
bool DataCharacters::parse(std::string url)
{
	doc.LoadFile(url.c_str());
	tinyxml2::XMLNode* m_xmlData;
	tinyxml2::XMLElement* m_elem;
	m_xmlData = (tinyxml2::XMLNode*)doc.FirstChildElement();
	m_xmlData = m_xmlData->FirstChild();

	DataCharacter* character;
	int m_nLevel;
	for (; m_xmlData != NULL; m_xmlData = m_xmlData->NextSiblingElement())
	{
		
		m_elem = m_xmlData->ToElement();

		character = new DataCharacter();

		m_nLevel = m_elem->IntAttribute("Level");
		character->SetExperience(m_elem->IntAttribute("Experience"));
		character->SetFatigue(m_elem->IntAttribute("Fatigue"));
		
		Character_Data[m_nLevel] = character;
	}
	return true;
}
