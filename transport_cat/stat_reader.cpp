#include "stat_reader.h"

// напишите решение с нуля
// код сохраните в свой git-репозиторий

namespace stat_reader {
	std::vector<std::string>& StatReader::GetBusesName()
	{
		return bus_name_queries;
	}
	StatReader Load(std::vector<std::string> queries)
	{
		StatReader stat;
		for (std::string query : queries) {
			size_t start_name_pos = query.find_first_not_of(' ', 3);
			stat.GetBusesName().push_back(query.substr(
				start_name_pos,
				query.find_last_not_of(' ') - start_name_pos + 1
			));
		}
		return stat;
	}

	void OutputStat(const std::string& name, size_t stop_count, size_t unique_stop_count, double distance)
	{
		if (stop_count == 0){
			std::cout << "Bus " << name << ": not found" << std::endl;
		}
		else {
			std::cout << std::setprecision(6) << "Bus " << name << ": " << stop_count << " stops on route, "
				<< unique_stop_count << " unique stops, " << distance << " route length" << std::endl;
		}
	}
	/*Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found*/
}