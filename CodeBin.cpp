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


#include <boost\lexical_cast.hpp>

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
	const static std::string strongLinkString = "strong";
	const static std::string weakLinkString = "weak";
	const static std::string nearLinkString = "near";
	const static std::string islandDataFile = "islands.dat";
	const static std::string linkDataFile = "links.dat";

	typedef unsigned int IslandHandle;

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
	struct RaceProperties
	{
		int	startNode;
		int	finishNode;
	};	

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

		std::cout << "name = " << node.name << " terrain = " << terrainTypeString << "\n";
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

		std::cout << "node A name = " << link.nodeNameA << ", node B name = " << link.nodeNameB << " link = " << linkType << "\n";
		return link;
	}

	typedef adjacency_list<vecS, vecS, undirectedS, IslandProperties, LinkProperties, RaceProperties> Graph;
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
	typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

	class Archipelago2
	{
	public:
		bool FindIslandByName(const std::string& name, IslandHandle& island);

		void MakeGraph();
		void PrintVertexAndEdgeData();
		void PrintOutgoingEdges();
		void StackOverflow();
		
	private:
		void ReadVertiesFromFile();
		void ReadEdgesFromFile();
		std::map<std::string, IslandHandle> vertexDict;
		Graph g;
	};
}

void Evi::Archipelago2::MakeGraph()
{
	ReadVertiesFromFile();
	ReadEdgesFromFile();
}

void Evi::Archipelago2::ReadVertiesFromFile()
{
	// read from file if present
	try
	{
		// consume island data file
		std::ifstream is(islandDataFile);
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

		is = std::ifstream(linkDataFile);
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
		throw std::runtime_error(error.what() + std::string("Error in file: ") + islandDataFile + "\n");
	}
}

void Evi::Archipelago2::ReadEdgesFromFile()
{
	// Create an edge conecting those two vertices
	//edge_t e; bool b;
	//boost::tie(e,b) = boost::add_edge(u,v,g);
	//boost::tie(e,b) = boost::add_edge(u,v,g);

	//// Set the properties of a vertex and the edge
	//g[e].linkType = LinkType::Strong;
}

bool Evi::Archipelago2::FindIslandByName(const std::string& name, IslandHandle& island)
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

void Evi::Archipelago2::PrintVertexAndEdgeData()
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

void Evi::Archipelago2::PrintOutgoingEdges()
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






void Evi::Archipelago2::StackOverflow()
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

//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <iostream>                      // for std::cout
#include <utility>                       // for std::pair
#include <algorithm>                     // for std::for_each
#include <boost/utility.hpp>             // for boost::tie
#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

using namespace boost;
/*
template <class Graph> struct exercise_vertex {
  exercise_vertex(Graph& g_) : g(g_) { }
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  void operator()(const Vertex& v) const
  {
    using namespace boost;
    typename property_map<Graph, vertex_index_t>::type 
      vertex_id = get(vertex_index, g);
    std::cout << "vertex: " << get(vertex_id, v) << std::endl;

    // Write out the outgoing edges
    std::cout << "\tout-edges: ";
    typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
    typename graph_traits<Graph>::edge_descriptor e;
    for (boost::tie(out_i, out_end) = out_edges(v, g); 
         out_i != out_end; ++out_i)
    {
      e = *out_i;
      Vertex src = source(e, g), targ = target(e, g);
      std::cout << "(" << get(vertex_id, src)
                << "," << get(vertex_id, targ) << ") ";
    }
    std::cout << std::endl;

    // Write out the incoming edges    
    std::cout << "\tin-edges: ";
    typename graph_traits<Graph>::in_edge_iterator in_i, in_end;
    for (boost::tie(in_i, in_end) = in_edges(v, g); in_i != in_end; ++in_i)
    {
      e = *in_i;
      Vertex src = source(e, g), targ = target(e, g);
      std::cout << "(" << get(vertex_id, src)
                << "," << get(vertex_id, targ) << ") ";
    }
    std::cout << std::endl;

    // Write out all adjacent vertices    
    std::cout << "\tadjacent vertices: ";
    typename graph_traits<Graph>::adjacency_iterator ai, ai_end;
    for (boost::tie(ai,ai_end) = adjacent_vertices(v, g);  ai != ai_end; ++ai)
      std::cout << get(vertex_id, *ai) <<  " ";
    std::cout << std::endl;
  }
  Graph& g;
};
*/
int foomain(int,char*[])
{
/*
	// create a typedef for the Graph type
  typedef adjacency_list<vecS, vecS, bidirectionalS, no_property, property<edge_weight_t, float> > Graph;

  // Make convenient labels for the vertices
  enum { A, B, C, D, E, N };
  const int num_vertices = N;
  const char* name = "ABCDE";

  // writing out the edges in the graph
  typedef std::pair<int,int> Edge;
  Edge edge_array[] = 
  { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
    Edge(C,E), Edge(B,D), Edge(D,E), };
  const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

  // average transmission delay (in milliseconds) for each connection
  float transmission_delay[] = { 1.2, 4.5, 2.6, 0.4, 5.2, 1.8, 3.3, 9.1 };

  // declare a graph object, adding the edges and edge properties
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ can't handle the iterator constructor
  Graph g(num_vertices);
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  for (std::size_t j = 0; j < num_edges; ++j) {
    graph_traits<Graph>::edge_descriptor e; bool inserted;
    tie(e, inserted) = add_edge(edge_array[j].first, edge_array[j].second, g);
    weightmap[e] = transmission_delay[j];
  }
#else
  Graph g(edge_array, edge_array + num_edges,
          transmission_delay, num_vertices);
#endif

  boost::property_map<Graph, vertex_index_t>::type 
    vertex_id = get(vertex_index, g);
  boost::property_map<Graph, edge_weight_t>::type
    trans_delay = get(edge_weight, g);

  std::cout << "vertices(g) = ";
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first)
    std::cout << name[get(vertex_id, *vp.first)] <<  " ";
  std::cout << std::endl;
  
  std::cout << "edges(g) = ";
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei,ei_end) = edges(g); ei != ei_end; ++ei)
    std::cout << "(" << name[get(vertex_id, source(*ei, g))]
              << "," << name[get(vertex_id, target(*ei, g))] << ") ";
  std::cout << std::endl;
  
  std::for_each(vertices(g).first, vertices(g).second,
                exercise_vertex<Graph>(g));
  
  std::map<std::string,std::string> graph_attr, vertex_attr, edge_attr;
  graph_attr["size"] = "3,3";
  graph_attr["rankdir"] = "LR";
  graph_attr["ratio"] = "fill";
  vertex_attr["shape"] = "circle";

  boost::write_graphviz(std::cout, g, 
                        make_label_writer(name),
                        make_label_writer(trans_delay),
                        make_graph_attributes_writer(graph_attr, vertex_attr, 
                                                     edge_attr));
  */
  std::cin.get();
  return 0;
}