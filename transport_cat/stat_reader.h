#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

namespace stat_reader {
	std::string_view ParseToBusName(std::string_view query);
	void OutputStat(std::string_view, size_t stop_count, size_t unique_stop_count, double distance);
}