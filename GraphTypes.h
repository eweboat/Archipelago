#pragma once

#include <boost/config.hpp>
#include <iostream>                      // for std::cout
#include <utility>                       // for std::pair
#include <algorithm>                     // for std::for_each
#include <boost/utility.hpp>             // for boost::tie
#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include <boost/algorithm/string.hpp>
#include <regex>
#include <string>
#include <boost\tokenizer.hpp>
#include <boost\bind.hpp>

enum class LinkType { Weak, Near, Strong };
enum class TerrainType { Grassy, Mountain, Swamp};

typedef unsigned int VertexHandle;

struct IslandProperties;
struct LinkProperties;
struct LinkData;
struct RaceProperties;

const static std::string grassyTerrainString = "grassy";
const static std::string mountainousTerrainString = "mountainous";
const static std::string swampyTerrainString = "swampy";
const static std::string strongLinkString = "strong";
const static std::string weakLinkString = "weak";
const static std::string nearLinkString = "near";
const static std::string islandDataFile = "islands.dat";
const static std::string linkDataFile = "links.dat";


	

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
	VertexHandle	resolvedNodeA;
	VertexHandle	resolvedNodeB;
	LinkProperties	properties;
};	
struct RaceProperties
{
	int	startNode;
	int	finishNode;
};