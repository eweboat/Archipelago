#pragma once
#include "GraphTypes.h"

enum LocomotionMechanism { Legs, Wheels, Wings };

class VehicleBase
{
public:
	void Reset(IslandHandle island);
	IslandHandle GetIsland() const;

private:
	IslandHandle		m_currentIsland;
		
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