#pragma once
#include <string>

typedef unsigned int IslandHandle;
enum class LinkType { Weak, Near, Strong };
enum class TerrainType { Grassy, Mountain, Swamp};

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
struct RaceLegProperties
{
	TerrainType		terrain;
	LinkType		linkType;
	IslandHandle	targetNode;
};

