#pragma once
#include <vector>
#include "GraphTypes.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class:			VehicleBase provides public interface to allow race interaction
//  Specialisation: Derived classes must implement to the routing methods and by doing
//					so define the node graph traversal rules
//  Comments:		vehicles do not presently store name and so individual vehicles can not
//					be easily differentiated except by memory address
//
/////////////////////////////////////////////////////////////////////////////////////

class VehicleBase
{
public:
	void Reset(IslandHandle location, IslandHandle target);
	
	// navigation / position
	void ChooseNextIsland(const std::vector<RaceLegProperties>& routeData);
	bool IsRouteValid(const RaceLegProperties& route) const { return IsRouteValidImpl(route); }
	IslandHandle GetIsland() const;
	
	// will apply direct or indirect movement penalties
	void ApplyMovementPenalties(const RaceLegProperties& route) { ApplyMovementPenaltiesImpl(route); }
	void SkipNextMove() { m_missNextMove = true; }
	
	std::string ToString() const { return ToStringImpl(); }

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

