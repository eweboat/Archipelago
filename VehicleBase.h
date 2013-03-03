#pragma once
#include <vector>
#include "GraphTypes.h"

enum LocomotionMechanism { Legs, Wheels, Wings };

class VehicleBase
{
public:
	void Reset(IslandHandle location, IslandHandle target);
	IslandHandle GetIsland() const;
	void ChooseNextIsland(const std::vector<RaceLegProperties>&) {m_currentIsland= m_targetIsland; }

private:
	IslandHandle		m_currentIsland;
	IslandHandle		m_targetIsland;
		
};

class WalkingVehicle : public VehicleBase
{
};

class DrivingVehicle : public VehicleBase
{
};

class FlyingVehicle : public VehicleBase
{
};