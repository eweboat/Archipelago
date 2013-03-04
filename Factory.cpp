#include "Factory.h"
#include <iostream>
#include <algorithm>
#include <boost\tokenizer.hpp>
#include "Vehicle.h"

// again it's a bit ugle having this data floating around like this, would prefer to read from data schedma
const static std::string grassyTerrainString = "grassy";
const static std::string mountainousTerrainString = "mountainous";
const static std::string swampyTerrainString = "swampy";
const static std::string strongLinkString = "strong";
const static std::string weakLinkString = "weak";
const static std::string nearLinkString = "near";

std::vector<std::string> TokeniseString(const std::string& data)
{
	std::vector<std::string> tokens;
	boost::tokenizer<> tokenizer(data);
	for(boost::tokenizer<>::iterator it=tokenizer.begin(); it != tokenizer.end(); ++it)
	{
		tokens.push_back(*it);
	}
	return tokens;
}


// for a data string extract node data values
// throws if cannot execute
IslandProperties ExtractIslandData(const std::string& data)
{
	// split data line into token
	std::vector<std::string> tokens = TokeniseString(data);
	// fail if cannot extract correct number of tokens
	if (tokens.size() != 2)
	{
		throw std::runtime_error("invalid data size: " + data + "\n");
	}

	// extract data to node
	IslandProperties node;
	node.name = tokens.at(0);
	std::string terrainTypeString(tokens.at(1));
	// perform lower case string comparision to map terrain type to enum
	std::transform(terrainTypeString.begin(), terrainTypeString.end(), terrainTypeString.begin(), ::tolower);
	if (terrainTypeString == grassyTerrainString)
	{
		node.terrain = TerrainType::Grassy;
	}
	else if (terrainTypeString == mountainousTerrainString)
	{
		node.terrain = TerrainType::Mountain;
	}
	else if (terrainTypeString == swampyTerrainString)
	{
		node.terrain = TerrainType::Swamp;
	}
	else
	{
		throw std::runtime_error("invalid terrain type: " + terrainTypeString + "\n");
	}

	//std::cout << "name = " << node.name << " terrain = " << terrainTypeString << "\n";
	return node;
}

LinkData ExtractLinkData(const std::string& data)
{
	// split data line into token
	std::vector<std::string> tokens = TokeniseString(data);
	// fail if cannot extract correct number of tokens
	if (tokens.size() != 3)
	{
		throw std::runtime_error("invalid data size: " + data + "\n");
	}

	// extract data to node
	LinkData link;
	link.nodeNameA = tokens.at(0);
	link.nodeNameB = tokens.at(1);
	std::string linkType(tokens.at(2));
	// perform lower case string comparision to map terrain type to enum
	std::transform(linkType.begin(), linkType.end(), linkType.begin(), ::tolower);
	if (linkType == strongLinkString)
	{
		link.properties.linkType = LinkType::Strong;
	}
	else if (linkType == weakLinkString)
	{
		link.properties.linkType = LinkType::Weak;
	}
	else if (linkType == nearLinkString)
	{
		link.properties.linkType = LinkType::Near;
	}
	else
	{
		throw std::runtime_error("invalid link type: " + linkType + "\n");
	}

	//std::cout << "node A name = " << link.nodeNameA << ", node B name = " << link.nodeNameB << " link = " << linkType << "\n";
	return link;
}
