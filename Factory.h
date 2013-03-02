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

using namespace boost;
using namespace std;

// A Grassy 
// A B weak

namespace Evi
{
	enum class LinkType { Weak, Near, Strong };
	enum class TerrainType { Grassy, Mountain, Swamp};
	const static std::string grassyTerrainString = "grassy";
	const static std::string mountainousTerrainString = "mountainous";
	const static std::string swampyTerrainString = "swampy";
	const static std::string islandDataFile = "islands.dat";


	struct IslandPropertySet
	{
		std::string		name;
		TerrainType		terrain;
	};
	struct Link
	{
		LinkType		linkType;
	};	
	struct RaceProperties
	{
		int	startNode;
		int	finishNode;
	};	

	// for a data string extract node data values
	// throws if cannot execute
	void ExtractData(IslandPropertySet& node, const std::string& data)
	{
		// split data line into token
		std::vector<std::string> tokens;
		boost::tokenizer<> tokenizer(data);
		for(boost::tokenizer<>::iterator it=tokenizer.begin(); it != tokenizer.end(); ++it)
		{
			tokens.push_back(*it);
		}
		// fail if cannot extract correct number of tokens
		if (tokens.size() != 2)
		{
			throw std::runtime_error("invalid data size: " + data + "\n");
		}

		// extract data to node
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

		std::cout << "name = " << node.name << " terrain = " << terrainTypeString << "\n";
	}

	typedef adjacency_list<vecS, vecS, undirectedS, IslandPropertySet, Link, RaceProperties> Graph;
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

	class Archipelago
	{
	public:
		void FindIslandByName(const std::string& name);

		void MakeGraph();
		void PrintVertexAndEdgeData();
		void PrintOutgoingEdges();
		void StackOverflow();
		
	private:
		void ReadVertiesFromFile();
		void ReadEdgesFromFile();
		Graph g;
	};
}

void Evi::Archipelago::MakeGraph()
{
	ReadVertiesFromFile();
}

void Evi::Archipelago::ReadVertiesFromFile()
{
	// read from file if present
	try
	{
		std::ifstream is(islandDataFile);
		if (is.good())
		{
			// digest file
			std::istream_iterator<std::string> start(is), end;
			std::vector<std::string> islandDataStrings = std::vector<std::string>(start, end);
			// extact data to property set
			std::vector<IslandPropertySet> islandNodeData(islandDataStrings.size());
			auto islandNodeIter = islandNodeData.begin();
			for_each(islandDataStrings.begin(), islandDataStrings.end(), boost::bind(&ExtractData, *islandNodeIter++, _1));
			// insert data into graph
			for ( auto island : islandNodeData )
			{
				vertex_t u = boost::add_vertex(g);
				vertex_t v = boost::add_vertex(g);
				g[u].name = island.name;
				g[u].terrain = island.terrain;

				edge_t e; bool b;
				boost::tie(e,b) = boost::add_edge(u,v,g);
			}

		}
	}
	catch (std::runtime_error& error)
	{
		throw std::runtime_error(error.what() + std::string("Error in file: ") + islandDataFile + "\n");
	}

	FindIslandByName("A");
}

void Evi::Archipelago::ReadEdgesFromFile()
{
	// Create an edge conecting those two vertices
	edge_t e; bool b;
	//boost::tie(e,b) = boost::add_edge(u,v,g);
	//boost::tie(e,b) = boost::add_edge(u,v,g);

	// Set the properties of a vertex and the edge
	g[e].linkType = LinkType::Strong;
}



void Evi::Archipelago::StackOverflow()
{
	//Define a class that has the data you want to associate to every vertex and edge
	struct Vertex{ int foo;};
	struct Edge{std::string blah;};

	//Define the graph using those classes
	typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, Vertex, Edge > Graph;
	//Some typedefs for simplicity
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

	//Instanciate a graph
	Graph g;

	// Create two vertices in that graph
	vertex_t u = boost::add_vertex(g);
	vertex_t v = boost::add_vertex(g);

	// Create an edge conecting those two vertices
	edge_t e; bool b;
	boost::tie(e,b) = boost::add_edge(u,v,g);


	// Set the properties of a vertex and the edge
	g[u].foo = 42;
	g[e].blah = "Hello world";
}
//bool IsTrue(const graph_traits<Evi::Graph>::vertex_iterator&)
//{
//	return true;
//}

void Evi::Archipelago::FindIslandByName(const std::string& name)
{
	// TODO: Come back and clean up ... maybe use find_if
	// get the property map for vertex indices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);
	
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
	{
		std::cout << "looking for \"" << name << "\" found \"" << g[*vp.first].name << "\"\n";
		if (g[*vp.first].name == name)
		{
			int foo = 1;
		}
	}

	//vp = vertices(g);
	//auto findResult = std::find_if (vp.first, vp.second, IsTrue);
	//std::cout << std::endl;
}

void Evi::Archipelago::PrintVertexAndEdgeData()
{
	// get the property map for vertex indices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);
	
	Graph::vertex_descriptor v = *vertices(g).first;
	g[v].name = "grassy";
	int i = 0;
	Graph::edge_descriptor e = *edges(g).first;
	g[e].linkType = LinkType::Near;

	std::cout << "vertices(g) = ";
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
	{
		std::cout << index[*vp.first] <<  " ";

		Graph::vertex_descriptor v = *vertices(g).first;
		std::cout << g[v+i++].name << " ";
	}



	i = 0;
	std::cout << "edges(g) = ";
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
	{
		std::cout << "(" << index[source(*ei, g)]  << "," << index[target(*ei, g)] << ") ";
			
		//Graph::edge_descriptor e = *edges(g).first;
		//std::cout << g[e].custom << " ";
	}
	std::cout << std::endl;
}

void Evi::Archipelago::PrintOutgoingEdges()
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











void LegacyShit()
{
	/*
	// create a typedef for the Graph type
	
	// Make convenient labels for the vertices
	enum { A, B, C, D, E, F, G, H, I, J, K, L, M, N, EndVale };

	// declare a graph object
	const int num_vertices = EndVale;

	// writing out the edges in the graph
	typedef std::pair<int, int> Edge;
	std::vector<Edge> edges;
	edges.push_back( Edge(A,B) );
	edges.push_back( Edge(A,D) );
	edges.push_back( Edge(B,C) );
	edges.push_back( Edge(C,F) );
	edges.push_back( Edge(D,E) );
	edges.push_back( Edge(E,F) );
	edges.push_back( Edge(E, H) );
	edges.push_back( Edge(F, I) );
	edges.push_back( Edge(H, J) );
	edges.push_back( Edge(H, K) );
	edges.push_back( Edge(I, K) );
	edges.push_back( Edge(I, M) );
	edges.push_back( Edge(J, L) );
	edges.push_back( Edge(K, M) );
	edges.push_back( Edge(L, M) );
	edges.push_back( Edge(L, N) );

	g = Graph(edges.begin(), edges.end(), num_vertices);
	

	//typename property_map<Graph, vertex_index_t>::type vertex_id = get(vertex_index, g);

	g[0].name = "vertex 0" ;
	g[1].name = "vertex 1" ;
	g[2].name = "vertex 2" ;
	g[3].name = "vertex 3" ;

	Graph::edge_descriptor e = *boost::edges(g).first;
	//Graph::edge_bundled foo;
	g[e].custom = "edge 0";

	Graph::vertex_descriptor v = *vertices(g).first;
	std::cout << ":" << g[1].name << ":" ;
	*/
}