#include "Race.h"
#include "Archipelago.h"
#include "VehicleBase.h" // todo remove when done


Race::Race(const Archipelago& archipelago_, IslandHandle start, IslandHandle finish) :
	m_archipelago(archipelago_),
	m_start(start), 
	m_finish(finish)
{
}

void Race::AddVehicle(const std::string& type)
{
	// todo should probably create vehicle based on data ...
	m_vehicles.push_back(new DrivingVehicle);
	m_vehicles.push_back(new FlyingVehicle);
	m_vehicles.push_back(new WalkingVehicle);
}

void Race::StartRace()
{
	// put each vehicle on the starting island
	IslandHandle startIsland = m_start; // must be in local scope for lambda function
	for_each (m_vehicles.begin(), m_vehicles.end(), [startIsland](VehicleBase* vehicle)
		{	
			vehicle->Reset(startIsland); 
		}
	);
}

void Race::Tick()
{
	// put each vehicle on the starting island
	Archipelago	archipelago = m_archipelago;// must be in local scope for lambda function
	for_each (m_vehicles.begin(), m_vehicles.end(), [&archipelago](VehicleBase* vehicle)
		{	
			archipelago.Visit(*vehicle); 
		}
	);
}

bool Race::IsFinished()
{
	for ( auto vehicle : m_vehicles )
	{
		if (vehicle->GetIsland() == m_finish)
		{
			return true;
		}
	}
	return false;
}

void Race::PrintStatus() const
{
}
