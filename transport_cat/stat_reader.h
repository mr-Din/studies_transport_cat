#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

namespace stat_reader {
	std::string ParseToBusName(const std::string& query);
	void OutputStat(const std::string& name, size_t stop_count, size_t unique_stop_count, double distance);
}