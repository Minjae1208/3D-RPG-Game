#pragma once

#include <string>
#include <map>
#include "tinyxml2.h"

#include "../Manager/LevelManager.h"

class DataInteraction
{
public:
	tinyxml2::XMLDocument doc;
public:
	bool parse(std::string url, DataLevel & pDataLevel);
};