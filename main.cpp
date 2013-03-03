#include "Factory.h"
#include "Archipelago.h"

int main(int,char*[])
{
	try
	{
		Archipelago archi;
		archi.MakeGraph();
		archi.PrintVertexAndEdgeData();
	}
	catch (std::runtime_error& error)
	{
		std::cerr << "An unexpected error has occured.\n" << error.what() << "Exiting ...\n";
	}
	catch (...)
	{
		std::cerr << "An unexpected error has occured.\nExiting ...\n";
	}

	std::cin.get();
  return 0;
}
