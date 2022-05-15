#include "stat_reader.h"
#include "input_reader.h"

namespace catalogue {
	namespace stat_reader {

		std::string_view ParseToName(std::string_view query) {
			size_t first_spase = query.find_first_of(' ');
			size_t start_name_pos = query.find_first_not_of(' ', first_spase);
			return(query.substr(
				start_name_pos,
				query.find_last_not_of(' ') - start_name_pos + 1)
				);
		}

		void OutputStat(const BusInfo& bus_info, std::ostream& out) {
			if (bus_info.stops_count == 0) {
				out << "Bus " << bus_info.name << ": not found" << std::endl;
			}
			else {
				out << std::setprecision(6) << "Bus " << bus_info.name << ": "
					<< bus_info.stops_count << " stops on route, "
					<< bus_info.unique_stops_count << " unique stops, "
					<< bus_info.distance << " route length, "
					<< bus_info.curvature << " curvature" << std::endl;
			}
		}

		void OutputBusesForStop(std::string_view name, const StopInfo& stop_info, std::ostream& out) {
			out << "Stop " << name;
			if (stop_info.name.empty()) {
				out << ": not found" << std::endl;
			}
			else if (stop_info.stops_to_buses_.size() == 0) {
				out << ": no buses" << std::endl;
			}
			else {
				out << ": buses";
				for (auto& bus : stop_info.stops_to_buses_) {
					out << ' ' << bus;
				}
				out << std::endl;
			}
		}

		void DisplayInformation(TransportCatalogue& catalogue, std::vector<std::string>& stat, std::ostream& out) {
			for (const auto& query : stat) {

				if (input_reader::IsBusQuery(query)) {
					auto bus_info = catalogue.GetBusInfo(stat_reader::ParseToName(query));
					stat_reader::OutputStat(bus_info, out);
				}
				else {
					auto stop_name = stat_reader::ParseToName(query);
					auto stop_info = catalogue.GetBusesForStop(stop_name);
					stat_reader::OutputBusesForStop(stop_name, stop_info, out);
				}
			}
		}
	} // namespace stat_reader
} // namespace catalogue