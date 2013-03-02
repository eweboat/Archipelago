#pragma once

#include <boost/config.hpp>
#include <iostream>                      // for std::cout
#include <utility>                       // for std::pair
#include <algorithm>                     // for std::for_each
#include <boost/utility.hpp>             // for boost::tie
#include <boost/graph/graph_traits.hpp>  // for boost::graph_traits
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

using namespace boost;

namespace Evi
{
	struct Island
	{
		std::string custom;
	};
	struct Link
	{
		std::string custom;
	};	
	struct RaceProperties
	{
		int	startNode;
		int	finishNode;
	};	

	class Archipelago
	{
		typedef adjacency_list<vecS, vecS, undirectedS, Island, Link, RaceProperties> Graph;
	public:
		void MakeGraph();
		void PrintVertexAndEdgeData();
		void PrintOutgoingEdges();
	private:
		Graph g;
	};
}

void Evi::Archipelago::MakeGraph()
{
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

#ifdef ADD
	// add the edges to the graph object
	g = Graph(num_vertices);
	const int num_edges = edges.size();	
	for (int i = 0; i < num_edges; ++i)
	{
		add_edge(edges[i].first, edges[i].second, g);
	}
#else
	g = Graph(edges.begin(), edges.end(), num_vertices);
#endif
}

void Evi::Archipelago::PrintVertexAndEdgeData()
{
	// get the property map for vertex indices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);
	
	Graph::vertex_descriptor v = *vertices(g).first;
	g[v].custom = "grassy";
	int i = 0;
	Graph::edge_descriptor e = *edges(g).first;
	g[e].custom = "weak";

	std::cout << "vertices(g) = ";
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	std::pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
	{
		std::cout << index[*vp.first] <<  " ";

		Graph::vertex_descriptor v = *vertices(g).first;
		std::cout << g[v+i++].custom << " ";
	}
	std::cout << std::endl;

	i = 0;
	std::cout << "edges(g) = ";
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
	{
		//std::cout << "(" << index[source(*ei, g)]  << "," << index[target(*ei, g)] << ") ";
			
		Graph::edge_descriptor e = *edges(g).first;
		std::cout << g[e].custom << " ";
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