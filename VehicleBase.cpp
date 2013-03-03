#include "VehicleBase.h"
#include <random>
#include <chrono>

void VehicleBase::Reset(IslandHandle location, IslandHandle target)
{
	m_currentIsland = location;
	m_targetIsland = target;
}

IslandHandle VehicleBase::GetIsland() const
{
	return m_currentIsland;
}

bool IsRouteValid(const VehicleBase&, const RaceLegProperties&)
{
	return true;
}

void VehicleBase::ChooseNextIsland(const std::vector<RaceLegProperties>& routeData) 
{
	// throw if no avail routes
	if (routeData.empty())
	{
		throw std::runtime_error("No route data available");
	}

	// check validity of each route
	std::vector<RaceLegProperties> validRoutes;
	for (auto route : routeData)
	{
		bool isRouteValid = IsRouteValid(*this, route);
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
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<unsigned int> distribution(0, validRoutes.size()-1);
	unsigned int randomValidRouteIndex = distribution(generator);
	m_currentIsland = validRoutes.at(randomValidRouteIndex).targetNode; 
}