#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace input_reader {
	std::string ReadLine();
	int ReadLineWithNumber();
	std::vector<std::string> ParseQueriesToVector();

	using StopsQuery = std::unordered_map<std::string, std::pair<double, double>>;
	using BusesQuery = std::unordered_map<std::string, std::vector<std::string>>;

	struct InputReader {
		
		StopsQuery stops_query_;
		BusesQuery buses_query_;
	};

	InputReader Load();

	std::string ParseToName(const std::string& query, size_t start_name, size_t colon_pos);
	
	std::pair<double, double> ParseToCoordinates(const std::string& query,
		size_t colon_pos, size_t comma_pos);

	std::vector<std::string> ParseToStopsName (std::string& query, size_t colon_pos);
	
}














/*struct InputReader {
	std::unordered_map<std::string, std::pair<double, double>> stops_;
	std::unordered_map<std::string, std::vector<std::string>> buses;
};*/

/*std::vector<std::string> ParseQueriesToVector() {
	int query_count = ReadLineWithNumber();
	std::vector<std::string> queries;
	queries.reserve(query_count);
	for (int i = 0; i < query_count; ++i) {
		std::string query = ReadLine();
		queries.push_back(query);
	}
	return queries;
}*/

/*bool IsQueryForAddStop(const std::string& query) {
	return true;
}*/

/*void GetInputReader() {
	//InputReader input;
	std::vector<std::string> queries = ParseQueriesToVector();
	for (auto& query : queries) {
		// Если начинается на Stop
		if (std::string first = query.substr(0, 4);
			first == "Stop") {

		}
		// Если начинается на Bus
	}
}*/