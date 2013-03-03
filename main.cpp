#include "Factory.h"
#include "Archipelago.h"
#include "Race.h"
#include "GraphTypes.h"

// these data could be loaded from file but didn't have time to do so
const static std::string islandDataFile = "islands.dat";
const static std::string linkDataFile = "links.dat";
const static std::string raceDataStart = "A";
const static std::string raceDataFinish = "I";

int main(int,char*[])
{
	try
	{
		Archipelago archi(islandDataFile, linkDataFile);

		// translate start and finish island names to handles
		IslandHandle startIsland, finishIsland;
		if ( !archi.FindIslandByName(raceDataStart, startIsland) )
		{
			throw std::runtime_error("Failed to find race start: " + raceDataStart + "\n");
		}
		if ( !archi.FindIslandByName(raceDataFinish, finishIsland) )
		{
			throw std::runtime_error("Failed to find race finish: " + raceDataFinish + "\n");
		}

		// create race
		Race race(archi, startIsland, finishIsland);

		// todo data input
		race.AddVehicle("wheels");
		race.StartRace();
		while ( !race.IsFinished() )
		{
			race.PrintStatus();
			race.Tick();
		}
		race.PrintStatus();
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
