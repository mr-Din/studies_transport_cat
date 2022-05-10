#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

namespace stat_reader {
	class StatReader {
	public:
		std::vector<std::string>& GetBusesName();
		//void SetBusName();
	private:
		std::vector<std::string> bus_name_queries;
	};

	StatReader Load(std::vector<std::string> queries);
	void OutputStat(const std::string& name, size_t stop_count, size_t unique_stop_count, double distance);
}