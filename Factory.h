#pragma once
#include <map>
#include <string>
#include <vector>
#include "GraphTypes.h"

// functions that need a home ...
std::vector<std::string> TokeniseString(const std::string& data);
LinkData ExtractLinkData(const std::string& data);
IslandProperties ExtractIslandData(const std::string& data);