#include "Vehicle.h"
#include <random>
#include <chrono>

void VehicleBase::Reset(IslandHandle location, IslandHandle target)
{
	m_currentIsland = location;
	m_targetIsland = target;
	m_missNextMove = false;
}

IslandHandle VehicleBase::GetIsland() const
{
	return m_currentIsland;
}

void VehicleBase::ChooseNextIsland(const std::vector<RaceLegProperties>& routeData) 
{
	// throw if no avail routes
	if (routeData.empty())
	{
		throw std::runtime_error("No route data available");
	}
	if (m_missNextMove)
	{
		m_missNextMove = false;
		return;
	}

	// check validity of each route
	std::vector<RaceLegProperties> validRoutes;
	for (auto route : routeData)
	{
		bool isRouteValid = IsRouteValid(route);
		if (isRouteValid)
		{
			// if finish line is reachable then it will be chosen
			if (route.targetNode == m_targetIsland)
			{
				validRoutes.clear();
				validRoutes.push_back(route);
				break;
			}
			validRoutes.push_back(route);
		}
	}
	// error if no valid routes; vehicle stuck!
	if (validRoutes.empty())
	{
		throw std::runtime_error("No valid routes available");
	}

	// move to island secect at random from list of available options
	// note that random is not behaving as randomly as I would like, need to investigate ..
	unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<unsigned int> distribution(0, validRoutes.size()-1);
	unsigned int randomValidRouteIndex = distribution(generator);
	m_currentIsland = validRoutes.at(randomValidRouteIndex).targetNode; 

	// apply any penalties from route choice
	ApplyMovementPenalties(validRoutes.at(randomValidRouteIndex));
}

bool WalkingVehicle::IsRouteValidImpl(const RaceLegProperties& route) const
{
	bool isValid = true;
	isValid = isValid && route.linkType != LinkType::Near;
	isValid = isValid && route.linkType != LinkType::Weak;
	isValid = isValid && route.terrain != TerrainType::Mountain;
	return isValid;
}

bool DrivingVehicle::IsRouteValidImpl(const RaceLegProperties& route) const
{
	bool isValid = true;
	isValid = isValid && route.linkType != LinkType::Near;
	isValid = isValid && route.terrain != TerrainType::Swamp;
	return isValid;
}

bool FlyingVehicle::IsRouteValidImpl(const RaceLegProperties& route) const
{
	bool isValid = true;
	isValid = isValid && route.terrain != TerrainType::Mountain;
	return isValid;
}

void FlyingVehicle::ApplyMovementPenaltiesImpl(const RaceLegProperties& route)
{
	if (route.terrain == TerrainType::Swamp)
	{
		SkipNextMove();
	}
}