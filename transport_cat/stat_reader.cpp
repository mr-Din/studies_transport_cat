#include "stat_reader.h"

namespace stat_reader {

	std::string_view ParseToName(std::string_view query)
	{
		// следующий символ после Bus или Stop
		size_t first_spase = query.find_first_of(' ');
		size_t start_name_pos = query.find_first_not_of(' ', first_spase);
		return(query.substr(
			start_name_pos,
			query.find_last_not_of(' ') - start_name_pos + 1
		));
	}
	void OutputStat(const BusInfo& bus_info)
	{
		if (bus_info.stops_count == 0){
			std::cout << "Bus " << bus_info.name << ": not found" << std::endl;
		}
		else {
			std::cout << std::setprecision(6) << "Bus " << bus_info.name << ": "
				<< bus_info.stops_count << " stops on route, "
				<< bus_info.unique_stops_count << " unique stops, "
				<< bus_info.distance << " route length, "
				<< bus_info.curvature << " curvature" << std::endl;
		}
		//Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature
	}
	void OutputBusesForStop(std::string_view name, const StopInfo& stop_info)
	{
		std::cout << "Stop " << name;
		if (stop_info.name.empty()) {
			std::cout << ": not found" << std::endl;
		}
		else if (stop_info.stops_to_buses_.size()==0) {
			std::cout << ": no buses" << std::endl;
		}
		else {
			std::cout << ": buses";
			for (auto& bus : stop_info.stops_to_buses_) {
				std::cout << ' ' << bus;
			}
			std::cout << std::endl;
		}
	}
}