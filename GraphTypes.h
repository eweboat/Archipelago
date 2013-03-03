#pragma once
#include <string>

// todo strip down what should be here

enum class LinkType { Weak, Near, Strong };
enum class TerrainType { Grassy, Mountain, Swamp};

typedef unsigned int IslandHandle;

const static std::string grassyTerrainString = "grassy";
const static std::string mountainousTerrainString = "mountainous";
const static std::string swampyTerrainString = "swampy";
const static std::string strongLinkString = "strong";
const static std::string weakLinkString = "weak";
const static std::string nearLinkString = "near";

// todo knock these into shape
struct IslandProperties
{
	std::string		name;
	TerrainType		terrain;
};
struct LinkProperties
{
	LinkType		linkType;
};	
struct LinkData
{
	std::string		nodeNameA;
	std::string		nodeNameB;
	IslandHandle	resolvedNodeA;
	IslandHandle	resolvedNodeB;
	LinkProperties	properties;
};	


