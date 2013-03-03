#include "VehicleBase.h"

void VehicleBase::Reset(IslandHandle location, IslandHandle target)
{
	m_currentIsland = location;
	m_targetIsland = target;
}

IslandHandle VehicleBase::GetIsland() const
{
	return m_currentIsland;
}