#pragma once
#include <string>


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
#include <boost\tokenizer.hpp>
#include <boost\bind.hpp>


#include <boost\lexical_cast.hpp>

#include "GraphTypes.h"

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, IslandProperties, LinkProperties> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

// functions that need a home ...
//std::vector<std::string> TokeniseString(const std::string& data);
//LinkData ExtractLinkData(const std::string& data);
//IslandProperties ExtractIslandData(const std::string& data);

class Archipelago
{
public:
	Archipelago(const std::string& islandFile, const std::string& linkFile);
	bool FindIslandByName(const std::string& name, IslandHandle& island);

	void Visit(class VehicleBase& vehicle);

	void PrintVertexAndEdgeData() const;
	void PrintOutgoingEdges() const;
		
private:
	std::map<std::string, IslandHandle> vertexDict;
	Graph g;
};









