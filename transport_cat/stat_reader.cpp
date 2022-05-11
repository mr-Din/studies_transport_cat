#include "stat_reader.h"

namespace stat_reader {

	std::string ParseToBusName(const std::string& query) {
		size_t start_name_pos = query.find_first_not_of(' ', 3);
		return(query.substr(
			start_name_pos,
			query.find_last_not_of(' ') - start_name_pos + 1
		));
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
}