#pragma once
#include <vector>
#include "GraphTypes.h"

enum LocomotionMechanism { Legs, Wheels, Wings };

class VehicleBase
{
public:
	void Reset(IslandHandle location, IslandHandle target);
	IslandHandle GetIsland() const;
	void ChooseNextIsland(const std::vector<RaceLegProperties>& routeData);
	std::string ToString() const { return ToStringImpl(); }

private:
	virtual std::string ToStringImpl() const = 0;

	IslandHandle		m_currentIsland;
	IslandHandle		m_targetIsland;
};

class WalkingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Walker"; } 
};

class DrivingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Driver"; } 
};

class FlyingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Flyer"; } 
};