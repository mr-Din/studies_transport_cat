#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "log_duration.h"
#include "transport_catalogue.h"

namespace input_reader {

	struct DataForStop {
		std::string name;
		Coordinates coordinate;
	};
	struct DataForBus {
		std::string name;
		std::vector<std::string> stops;
	};
	std::string ReadLine();
	int ReadLineWithNumber();
	std::vector<std::string> ParseQueriesToVector();
	bool IsStopQuery(const std::string& query);
	bool IsBusQuery(const std::string& query);
	DataForStop ParseToStop(const std::string& query);
	DataForBus ParseToBus(const std::string& query);

	
	std::string ParseToName(const std::string& query, size_t start_name, size_t colon_pos);
	
	std::pair<double, double> ParseToCoordinates(const std::string& query,
		size_t colon_pos, size_t comma_pos);

	std::vector<std::string> ParseToStopsName (const std::string& query, size_t colon_pos);
}
