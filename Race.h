#pragma once

#include <vector>

class IslandBase;
class VehicleBase;

class Race
{
public:
	Race() 
	{
	}

private:
	std::vector<VehicleBase>	m_vehicles;
	const IslandBase&			m_start;
	const IslandBase&			m_finish;
}