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
	bool IsRouteValid(const RaceLegProperties& route) const { return IsRouteValidImpl(route); }
	void ApplyMovementPenalties(const RaceLegProperties& route) { ApplyMovementPenaltiesImpl(route); }
	void SkipNextMove() { m_missNextMove = true; }

private:
	virtual std::string ToStringImpl() const = 0;
	virtual bool IsRouteValidImpl(const RaceLegProperties& route) const = 0;
	virtual void ApplyMovementPenaltiesImpl(const RaceLegProperties& route) = 0;

	IslandHandle		m_currentIsland;
	IslandHandle		m_targetIsland;
	bool				m_missNextMove;
};

class WalkingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Walker"; } 
	bool IsRouteValidImpl(const RaceLegProperties& route) const;
	void ApplyMovementPenaltiesImpl(const RaceLegProperties& route) { };
};

class DrivingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Driver"; } 
	bool IsRouteValidImpl(const RaceLegProperties& route) const;
	void ApplyMovementPenaltiesImpl(const RaceLegProperties& route) { };
};

class FlyingVehicle : public VehicleBase
{
private:
	std::string ToStringImpl() const { return "Flyer"; } 
	bool IsRouteValidImpl(const RaceLegProperties& route) const;
	void ApplyMovementPenaltiesImpl(const RaceLegProperties& route);
};

