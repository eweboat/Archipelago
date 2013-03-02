#pragma once

#include <vector>

class IslandBase;
class VehicleBase;

namespace Evi
{
	class Race
	{
	public:
		Race(const IslandBase& start, const IslandBase&	finish);
	

	private:
		std::vector<VehicleBase*>	m_vehicles;
		const IslandBase&			m_start;
		const IslandBase&			m_finish;
	};
}