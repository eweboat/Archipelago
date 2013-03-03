#pragma once

#include <vector>
#include <string>
#include "GraphTypes.h"

class VehicleBase;
class Archipelago;

class Race
{
public:
	Race(const Archipelago& archipelago_, IslandHandle start, IslandHandle finish);
	void AddVehicle(const std::string& type);
	void StartRace();
	void Tick();
	bool IsFinished();
	void PrintStatus() const;

private:
	// disallow copy
	Race(const Race&); // = delete; // uncomment if compiler supports c++11
	Race& operator= (const Race&); // = delete;

private:
	const Archipelago&			m_archipelago;
	IslandHandle				m_start;
	IslandHandle				m_finish;
	std::vector<VehicleBase*>	m_vehicles;
};