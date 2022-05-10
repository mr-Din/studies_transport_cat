#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace input_reader {
	std::string ReadLine();
	int ReadLineWithNumber();
	std::vector<std::string> ParseQueriesToVector();


	class InputReader {
	public:
		std::pair<double, double>& AddStop(std::string name);
		std::vector<std::string>& AddBus(std::string name);

		std::unordered_map<std::string, std::pair<double, double>>& GetStopsQuery();
		std::unordered_map<std::string, std::vector<std::string>>& GetBusesQuery();

	private:
		std::unordered_map<std::string, std::pair<double, double>> stops_query_;
		std::unordered_map<std::string, std::vector<std::string>> buses_query_;
	};


	InputReader Load();

	std::vector<std::string> ParseQueryToStopsName(std::string& query);
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