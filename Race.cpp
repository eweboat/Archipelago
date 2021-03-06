#include "Race.h"
#include <string>
#include <boost\lexical_cast.hpp>
#include "Factory.h"
#include "Archipelago.h"
#include "Vehicle.h" 

const static std::string walkerVehicleString = "walker";
const static std::string driverVehicleString = "driver";
const static std::string flyerVehicleString = "flyer";

Race::Race(const Archipelago& archipelago_, IslandHandle start, IslandHandle finish) :
	m_archipelago(archipelago_),
	m_start(start), 
	m_finish(finish),
	m_raceStep(0),
	m_isFinished(false)
{
}

Race::~Race()
{
	// delete heap memory
	for ( auto vehicle : m_vehicles )
	{
		delete vehicle;
	}
}

void Race::AddVehicles(const std::vector<std::string>& vehicleTypes)
{
	for( auto data : vehicleTypes)
	{
		// split data line into token
		std::vector<std::string> tokens = TokeniseString(data);

		// extract data to node
		std::string vehicleType(tokens.at(0));
		// perform lower case string comparision to map terrain type to enum
		std::transform(vehicleType.begin(), vehicleType.end(), vehicleType.begin(), ::tolower);
		if (vehicleType == walkerVehicleString)
		{
		m_vehicles.push_back(new WalkingVehicle);
		}
		else if (vehicleType == driverVehicleString)
		{
			m_vehicles.push_back(new DrivingVehicle);
		}
		else if (vehicleType == flyerVehicleString)
		{
			m_vehicles.push_back(new FlyingVehicle);
		}
	}
}

void Race::StartRace()
{
	// put each vehicle on the starting island
	IslandHandle startIsland = m_start; // must be in local scope for lambda function
	IslandHandle finishIsland = m_finish; // must be in local scope for lambda function
	for_each (m_vehicles.begin(), m_vehicles.end(), [startIsland, finishIsland](VehicleBase* vehicle)
		{	
			vehicle->Reset(startIsland, finishIsland); 
		}
	);
	m_raceStep = 0;
	m_isFinished = false;
}

void Race::Tick()
{
	// put each vehicle on the starting island
	const Archipelago&	archipelago = m_archipelago;// must be in local scope for lambda function
	for_each (m_vehicles.begin(), m_vehicles.end(), [&archipelago](VehicleBase* vehicle)
		{	
			archipelago.Visit(*vehicle); 
		}
	);
	++m_raceStep;
	// test for victory
	IsFinished();
}

bool Race::IsFinished()
{
	// easy return is race is already marked as finished
	if (m_isFinished)
	{
		return m_isFinished;
	}
	// otherwise scan for victory condition
	for ( auto vehicle : m_vehicles )
	{
		if (vehicle->GetIsland() == m_finish)
		{
			m_isFinished = true;
			return m_isFinished;
		}
	}
	return m_isFinished;
}

std::string Race::GetStatusReport() const
{
	// format race data for print to standard out
	std::string finishIslandName;
	m_archipelago.FindIslandNameByHandle(m_finish, finishIslandName);
	std::string header("Race to \""+ finishIslandName +"\" at step " + boost::lexical_cast<std::string>(m_raceStep));
	std::string result = header + "\n" + std::string (header.size(), '-') + "\n";

	for (auto vehicle : m_vehicles )
	{
		std::string islandName("NOT FOUND");
		m_archipelago.FindIslandNameByHandle(vehicle->GetIsland(), islandName);
		result +=  vehicle->ToString() + "("+ boost::lexical_cast<std::string>(vehicle) +") is on island \""+ islandName +"\" \n";
	}

	if (m_isFinished)
	{
		result +=  "RACE HAS BEEN WON!!\n";
	}
	result += "\n";
	return result;
}
