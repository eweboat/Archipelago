#include "Archipelago.h"
#include "Factory.h"
#include <string>

#include "Vehicle.h"

Archipelago::Archipelago(const std::string& islandFile, const std::string& linkFile)
{
	// read from file if present
	std::string currentFileRead = islandFile;
	try
	{
		// consume island data file
		std::ifstream is(islandFile);
		if (is.good())
		{
			// digest file
			std::istream_iterator<std::string> start(is), end;
			std::vector<std::string> islandDataStrings = std::vector<std::string>(start, end);
			// extact data to property set
			std::vector<IslandProperties> islandNodes(islandDataStrings.size());
			std::transform (islandDataStrings.begin(), islandDataStrings.end(), islandNodes.begin(), ExtractIslandData);
			// insert data into graph
			for ( auto island : islandNodes )
			{
				vertex_t u = boost::add_vertex(m_islandGraph);
				m_islandGraph[u].name = island.name;
				m_islandGraph[u].terrain = island.terrain;
			}
		}

		currentFileRead = linkFile;
		is = std::ifstream(linkFile);
		if (is.good())
		{
			// digest file
			std::istream_iterator<std::string> start(is), end;
			std::vector<std::string> linkDataStrings = std::vector<std::string>(start, end);
			// extact data to property set
			std::vector<LinkData> linkData(linkDataStrings.size());
			std::transform (linkDataStrings.begin(), linkDataStrings.end(), linkData.begin(), ExtractLinkData);
			// insert data into graph
			for ( auto link : linkData )
			{
				unsigned int searchResultA;
				unsigned int searchResultB;
				if (FindIslandByName(link.nodeNameA, searchResultA) && FindIslandByName(link.nodeNameB, searchResultB))
				{
					link.resolvedNodeA = searchResultA;
					link.resolvedNodeB = searchResultB;
				}
				else
				{
					throw std::runtime_error("Error: could not find one of " + link.nodeNameA + " or " + link.nodeNameB + "in island nodes\n");
				}

				// Create an edge conecting those two vertices
				edge_t e; bool b;
				Graph::vertex_descriptor u = *vertices(m_islandGraph).first + link.resolvedNodeA;
				Graph::vertex_descriptor v = *vertices(m_islandGraph).first + link.resolvedNodeB;
				boost::tie(e,b) = boost::add_edge(u,v,m_islandGraph);

				// Set the properties of a vertex and the edge
				m_islandGraph[e].linkType = link.properties.linkType;
			}
		}
	}
	catch (std::runtime_error& error)
	{
		throw std::runtime_error(error.what() + std::string("Error in file: ") + currentFileRead + "\n");
	}
}

bool Archipelago::FindIslandByName(const std::string& name, IslandHandle& island)
{
	// search class dict for mapping
	auto searchResult = m_vertexDict.find(name);
	if (searchResult != m_vertexDict.end())
	{
		island = searchResult->second;
		return true;
	}

	// if key not in dict then find through search
	// get the property map for vertex indices
	graph_traits<Graph>::vertex_iterator it, end;
	for (boost::tie( it, end ) = vertices(m_islandGraph); it != end; ++it)
	{
		if (m_islandGraph[*it].name == name)
		{
			island = *it;
			m_vertexDict[name] = island;
			return true;
		}
	}
	return false;
}

bool Archipelago::FindIslandNameByHandle(IslandHandle island, std::string& name) const
{
	// scan key value pair for match
	for ( auto kv : m_vertexDict )
	{
		if (kv.second == island)
		{
			name = kv.first;
			return true;
		}
	}
	return false;
}


void Archipelago::Visit(VehicleBase& vehicle) const
{
	try
	{
		// get the property map for vertex indices
		typedef property_map<Graph, vertex_index_t>::type IndexMap;
		IndexMap index = get(vertex_index, m_islandGraph);
	
		// for each adjacent node find route data
		std::vector<RaceLegProperties> availableLinks;
		auto adjacentEdges = m_islandGraph.out_edge_list(vehicle.GetIsland());
		for (auto link : adjacentEdges)
		{
			// find edge connecting location and adjacent node
			edge_t edge; bool edgeFound;
			Graph::vertex_descriptor currentIsland = *vertices(m_islandGraph).first + vehicle.GetIsland();
			Graph::vertex_descriptor adjacentIsland = *vertices(m_islandGraph).first + link.m_target;
			boost::tie(edge, edgeFound) = boost::edge(currentIsland, adjacentIsland, m_islandGraph);
		
			// copy data route data to be passed to vehicle
			if ( edgeFound )
			{
				RaceLegProperties leg;
				leg.linkType = m_islandGraph[edge].linkType;
				leg.targetNode = link.m_target;
				leg.terrain = m_islandGraph[leg.targetNode].terrain;
				availableLinks.push_back(leg);
			}
		}

		// pass all available routes to vehicle
		vehicle.ChooseNextIsland(availableLinks);
	}
	catch (std::exception& error)
	{
		std::cerr << vehicle.ToString() << " has a problem: " << error.what() << "\n";
	}
}

