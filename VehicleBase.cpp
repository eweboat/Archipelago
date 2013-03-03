#include "VehicleBase.h"

void VehicleBase::Reset(IslandHandle island)
{
	m_currentIsland = island;
}

IslandHandle VehicleBase::GetIsland() const
{
	return m_currentIsland;
}