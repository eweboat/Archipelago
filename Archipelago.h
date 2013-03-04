#pragma once
#include <string>
#include <map>
#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include "GraphTypes.h"


/////////////////////////////////////////////////////////////////////////////////////
//
//  Class:			Archipelago represent the island and connections between them
//					using the boost graph library of adjacent nodes
//  Comments:		Custom data is attached to verticies (islands) and edges (bridges)
//					by the bundled property attribute
//
/////////////////////////////////////////////////////////////////////////////////////


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, IslandProperties, LinkProperties> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

class Archipelago
{
public:
	Archipelago(const std::string& islandFile, const std::string& linkFile);

	bool FindIslandByName(const std::string& name, IslandHandle& island);
	bool FindIslandNameByHandle(IslandHandle island, std::string& name) const;

	void Visit(class VehicleBase& vehicle) const;

private:
	typedef std::map<std::string, IslandHandle>	VertexDict;
	VertexDict								m_vertexDict;
	Graph									m_islandGraph;
};









