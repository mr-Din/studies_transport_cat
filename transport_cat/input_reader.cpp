#include "input_reader.h"

namespace input_reader {
	std::string ReadLine() {
		std::string s;
		std::getline(std::cin, s);
		return s;
	}

	int ReadLineWithNumber() {
		int result;
		std::cin >> result;
		ReadLine();
		return result;
	}

	std::vector<std::string> ParseQueriesToVector() {
		int query_count = ReadLineWithNumber();
		std::vector<std::string> queries;
		queries.reserve(query_count);
		for (int i = 0; i < query_count; ++i) {
			std::string query = ReadLine();
			queries.push_back(move(query));
		}
		return queries;
	}

	std::pair<double, double>& InputReader::AddStop(std::string name) {
		return stops_query_[move(name)];
	}

	std::vector<std::string>& InputReader::AddBus(std::string name)
	{
		return buses_query_[move(name)];
	}

	std::unordered_map<std::string, std::pair<double, double>>& InputReader::GetStopsQuery()
	{
		return stops_query_;
	}

	std::unordered_map<std::string, std::vector<std::string>>& InputReader::GetBusesQuery()
	{
		return buses_query_;
	}

	InputReader Load()
	{
		std::vector<std::string> queries = ParseQueriesToVector();

		InputReader input;

		for (std::string& query : queries) {
			// Если запрос на создание остановки
			if (!query.empty() && query[0] == 'S') {

				size_t colon_pos = query.find_first_of(':');
				size_t comma_pos = query.find_first_of(',');
				size_t start_name = query.find_first_not_of(' ', 4);

				std::string stop_name = query.substr(
					start_name,
					query.find_last_not_of(' ', colon_pos - 1) - start_name + 1);
				//std::cout <<'!'<< stop_name<<'!' << std::endl;

				double lat = std::stod(query.substr(
					colon_pos + 1,
					comma_pos - colon_pos
				));
				double lng = std::stod(query.substr(
					comma_pos + 1
				));
				//std::cout << '!' << lat << '!' << lng << '!' << std::endl;
				input.AddStop(move(stop_name)) = { lat, lng };
			}
			else if (!query.empty() && query[0] == 'B') {
				size_t colon_pos = query.find_first_of(':');
				size_t start_name = query.find_first_not_of(' ', 3);

				std::string bus_name = query.substr(
					start_name,
					query.find_last_not_of(' ', colon_pos - 1) - start_name + 1);
				//std::cout << '!' << bus_name << '!' << std::endl;
				std::vector<std::string> stops; // = ParseQueryToStopsName (string& query)
				if (size_t separator_pos = query.find('>'); separator_pos != std::string::npos) {
					size_t start_stop_name_pos = query.find_first_not_of(' ', colon_pos + 1);
					while (separator_pos != std::string::npos) {
						std::string stop_name = query.substr(
							start_stop_name_pos,
							query.find_last_not_of(' ', separator_pos - 1) - start_stop_name_pos + 1);
						stops.push_back(move(stop_name));

						start_stop_name_pos = query.find_first_not_of(' ', separator_pos + 1);
						separator_pos = query.find_first_of('>', separator_pos + 1);
					}
					std::string stop_name = query.substr(
						start_stop_name_pos,
						query.find_last_not_of(' ') - start_stop_name_pos + 1);
					stops.push_back(move(stop_name));
					//std::cout << start_stop_name_pos << std::endl;
					//input.AddBus(move(bus_name)) = move(stops);
				}
				/*for (auto& c : stops) {
					std::cout << '!' << c << '!' << std::endl;
				}*/
				if (size_t separator_pos = query.find('-'); separator_pos != std::string::npos) {
					size_t start_stop_name_pos = query.find_first_not_of(' ', colon_pos + 1);
					while (separator_pos != std::string::npos) {
						std::string stop_name = query.substr(
							start_stop_name_pos,
							query.find_last_not_of(' ', separator_pos - 1) - start_stop_name_pos + 1);
						stops.push_back(move(stop_name));

						start_stop_name_pos = query.find_first_not_of(' ', separator_pos + 1);
						separator_pos = query.find_first_of('-', separator_pos + 1);
					}
					std::string stop_name = query.substr(
						start_stop_name_pos,
						query.find_last_not_of(' ') - start_stop_name_pos + 1);
					stops.push_back(move(stop_name));
					for (size_t i = stops.size() - 1; i > 0; --i) {
						stops.push_back(stops[i - 1]);
					}
					//std::cout << start_stop_name_pos << std::endl;
					//input.AddBus(move(bus_name)) = move(stops);
				}
				input.AddBus(move(bus_name)) = move(stops);

			}

		}

		return input;
	}
	std::vector<std::string> ParseQueryToStopsName(std::string& query)
	{
		return std::vector<std::string>();
	}
}