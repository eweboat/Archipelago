#include "Archipelago.h"
#include "Factory.h"
#include <string>

#include "VehicleBase.h"

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
				vertex_t u = boost::add_vertex(g);
				g[u].name = island.name;
				g[u].terrain = island.terrain;
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
				Graph::vertex_descriptor u = *vertices(g).first + link.resolvedNodeA;
				Graph::vertex_descriptor v = *vertices(g).first + link.resolvedNodeB;
				boost::tie(e,b) = boost::add_edge(u,v,g);

				// Set the properties of a vertex and the edge
				g[e].linkType = link.properties.linkType;
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
	auto searchResult = vertexDict.find(name);
	if (searchResult != vertexDict.end())
	{
		island = searchResult->second;
		return true;
	}

	// if key not in dict then find through search
	// get the property map for vertex indices
	graph_traits<Graph>::vertex_iterator it, end;
	for (boost::tie( it, end ) = vertices(g); it != end; ++it)
	{
		if (g[*it].name == name)
		{
			island = *it;
			vertexDict[name] = island;
			return true;
		}
	}
	return false;
}

void Archipelago::PrintVertexAndEdgeData() const
{
	std::cout << "vertices(g) = ";
	graph_traits<Graph>::vertex_iterator it, end;
	for (boost::tie( it, end ) = vertices(g); it != end; ++it)
	{
		std::string terrainString = (g[*it].terrain == TerrainType::Grassy) ? "grassy" : "mountain";
		terrainString = g[*it].terrain == TerrainType::Swamp ? "swamp" : terrainString;
		std::cout << g[*it].name << ":" << terrainString << " ";
	}

	std::cout << "edges(g) = ";
	// get the property map for vertex indices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);
	graph_traits<Graph>::edge_iterator ei, ei_end;

	for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
	{
		// TODO :: this gives me access to a nodes links !! :)
		auto foo = g.out_edge_list(0);
		g.m_edges;
		//std::string foo = g.m_edges[source(*ei, g)] == LinkType::Strong ? "strong" : "poo";
		std::cout << "(" << index[source(*ei, g)]  << "," << index[target(*ei, g)] << ") ";
	}
	std::cout << std::endl;
}

void Archipelago::PrintOutgoingEdges() const
{
	//// Write out the outgoing edges
	//std::cout << "\tout-edges: ";
	//typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
	//typename graph_traits<Graph>::edge_descriptor e;
	//for (boost::tie(out_i, out_end) = out_edges(v, g); 
	//out_i != out_end; ++out_i)
	//{
	//	e = *out_i;
	//	Vertex src = source(e, g), targ = target(e, g);
	//	std::cout << "(" << get(vertex_id, src) << "," << get(vertex_id, targ) << ") ";
	//}
	//std::cout << std::endl;
}

void Archipelago::Visit(VehicleBase& vehicle)
{
	vehicle.ChooseNextIsland();
}


