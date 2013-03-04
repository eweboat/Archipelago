#include <fstream>
#include "Factory.h"
#include "Archipelago.h"
#include "Race.h"
#include "GraphTypes.h"

// these data could be loaded from the command line or file but didn't have time to do so..
const static std::string islandDataFile = "islands.dat";
const static std::string linkDataFile = "links.dat";
const static std::string vehicleDataFile = "vehicles.dat";
const static std::string raceDataStart = "A";
const static std::string raceDataFinish = "I";

int main(int,char*[])
{
	try
	{
		Archipelago islandArchipelago(islandDataFile, linkDataFile);

		// translate start and finish island names to handles
		IslandHandle startIsland, finishIsland;
		if ( !islandArchipelago.FindIslandByName(raceDataStart, startIsland) )
		{
			throw std::runtime_error("Failed to find race start: " + raceDataStart + "\n");
		}
		if ( !islandArchipelago.FindIslandByName(raceDataFinish, finishIsland) )
		{
			throw std::runtime_error("Failed to find race finish: " + raceDataFinish + "\n");
		}

		// create race object
		Race race(islandArchipelago, startIsland, finishIsland);

		// read in compeating vehicles
		std::ifstream is(vehicleDataFile);
		if (is.good())
		{
			// digest file
			std::istream_iterator<std::string> start(is), end;
			std::vector<std::string> vehicleDataStrings = std::vector<std::string>(start, end);
			race.AddVehicles(vehicleDataStrings);
		}
		else
		{
			throw std::runtime_error("could not open vehicles file: " + vehicleDataFile);
		}

		// ... and start the race!
		race.StartRace();
		while ( !race.IsFinished() )
		{
			std::cout << race.GetStatusReport();
			race.Tick();
		}
		std::cout << race.GetStatusReport();
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
