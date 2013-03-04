#pragma once

#include <vector>
#include <string>
#include "GraphTypes.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class:	Race holds state data for the race and the provides access to island node
//			graph for the vehicles in the race.
//
/////////////////////////////////////////////////////////////////////////////////////


class VehicleBase;
class Archipelago;

class Race
{
public:
	Race(const Archipelago& archipelago_, IslandHandle start, IslandHandle finish);
	~Race();

	void AddVehicles(const std::vector<std::string>& vehicleTypes);
	void StartRace();
	void Tick();
	bool IsFinished();
	std::string GetStatusReport() const;

private:
	// disallow copy
	Race(const Race&); // = delete; // uncomment if compiler supports c++11
	Race& operator= (const Race&); // = delete;

private:
	const Archipelago&			m_archipelago;
	IslandHandle				m_start;
	IslandHandle				m_finish;
	std::vector<VehicleBase*>	m_vehicles;
	unsigned int				m_raceStep;
	bool						m_isFinished;
};