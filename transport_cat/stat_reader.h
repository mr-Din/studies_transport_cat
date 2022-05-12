#pragma once
#include "transport_catalogue.h"

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

namespace stat_reader {
	std::string_view ParseToName(std::string_view query);
	void OutputStat(const BusInfo& bus_info);
	void OutputBusesForStop(std::string_view name, const StopInfo& stop_info);
}