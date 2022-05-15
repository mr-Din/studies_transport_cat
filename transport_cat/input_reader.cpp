#include "input_reader.h"

using namespace std;

namespace catalogue {
	namespace input_reader {
		namespace detail {
			string ReadLine(std::istream& input_query) {
				string s;
				getline(input_query, s);
				return s;
			}

			int ReadLineWithNumber(std::istream& input_query) {
				int result;
				input_query >> result;
				ReadLine(input_query);
				return result;
			}

			
		} // namespace detail

		vector<string> ParseQueriesToVector(std::istream& input_query) {
			int query_count = detail::ReadLineWithNumber(input_query);
			vector<string> queries;
			queries.reserve(query_count);
			for (int i = 0; i < query_count; ++i) {
				string query = detail::ReadLine(input_query);
				queries.push_back(move(query));
			}
			return queries;
		}

		bool IsStopQuery(string_view query) {
			return !query.empty() && query[0] == 'S';
		}

		bool IsBusQuery(string_view query) {
			return !query.empty() && query[0] == 'B';
		}

		DataForStop ParseToStop(string_view query) {
			size_t start_name = query.find_first_not_of(' ', 4);
			size_t colon_pos = query.find_first_of(':');
			auto coordinates = ParseToCoordinates(query, colon_pos);
			return { ParseToName(query, start_name, colon_pos),
				 coordinates.lat, coordinates.lng };
		}

		DataForBus ParseToBus(string_view query) {
			size_t start_name = query.find_first_not_of(' ', 3);
			size_t colon_pos = query.find_first_of(':');

			return { ParseToName(query, start_name, colon_pos),
				ParseToStopsName(query, colon_pos) };
		}

		string_view ParseToName(string_view query, size_t start_name, size_t colon_pos) {
			string_view stop_name = query.substr(
				start_name,
				query.find_last_not_of(' ', colon_pos - 1) - start_name + 1);
			return stop_name;
		}

		Coordinates ParseToCoordinates(string_view query,
			size_t colon_pos) {

			size_t comma_pos = query.find_first_of(',');
			double lat = stod(string(query).substr(
				colon_pos + 1, comma_pos - colon_pos)
			);

			size_t comma_pos_next = query.find_first_of(',', comma_pos + 1);
			double lng = stod(string(query).substr(
				comma_pos + 1, comma_pos_next)
			);

			return { lat, lng };
		}

		vector<string_view> ParseToStopsName(string_view query, size_t colon_pos) {
			vector<string_view> stops;

			if (size_t separator_pos = query.find_first_of(">-"); separator_pos != string::npos) {
				size_t start_stop_name_pos = query.find_first_not_of(' ', colon_pos + 1);
				while (separator_pos != string::npos) {
					string_view stop_name = query.substr(
						start_stop_name_pos,
						query.find_last_not_of(' ', separator_pos - 1) - start_stop_name_pos + 1
					);

					stops.push_back(move(stop_name));

					start_stop_name_pos = query.find_first_not_of(' ', separator_pos + 1);
					separator_pos = query.find_first_of(">-", separator_pos + 1);
				}
				string_view stop_name = query.substr(
					start_stop_name_pos,
					query.find_last_not_of(' ') - start_stop_name_pos + 1
				);

				stops.push_back(move(stop_name));
			}
			if (query.find('-') != string::npos) {
				for (size_t i = stops.size() - 1; i > 0; --i) {
					stops.push_back(stops[i - 1]);
				}
			}
			return stops;
		}
		vector<DataForDistance> ParseToDistanceToStops(std::string_view query) {
			vector<DataForDistance> distances;
			size_t start_name = query.find_first_not_of(' ', 4);
			size_t colon_pos = query.find_first_of(':');
			auto stop_name_from = ParseToName(query, start_name, colon_pos);

			size_t pos_begin = query.find_first_of(',', query.find_first_of(',') + 1) + 1;
			pos_begin = query.find_first_not_of(' ', pos_begin);

			while (pos_begin != string::npos && pos_begin != 0) {
				size_t pos_end = query.find_first_of('m', pos_begin);
				int distance = stoi(string(query).substr(pos_begin, pos_end - pos_begin));

				pos_begin = query.find_first_not_of(' ', query.find("to"s, pos_end) + 2);
				pos_end = query.find_last_not_of(' ', query.find_first_of(',', pos_end) - 1);

				string_view stop_name_to = query.substr(pos_begin, pos_end - pos_begin + 1);

				pos_begin = query.find_first_not_of(' ', query.find_first_of(',', pos_end) + 1);
				distances.push_back({ stop_name_from, stop_name_to, distance });
			}
			return distances;
		}

		void FillTransportCatalogue(TransportCatalogue& catalogue, vector<string>& input) {
			sort(input.begin(), input.end(), std::greater<std::string>());

			vector<string_view> names_of_stops;
			for (const auto& query : input) {
				if (input_reader::IsStopQuery(query)) {
					auto data_for_stop = input_reader::ParseToStop(query);
					catalogue.AddStop(data_for_stop.name, data_for_stop.coordinate);
					names_of_stops.push_back(data_for_stop.name);
				}
				else if (input_reader::IsBusQuery(query)) {
					auto data_for_bus = input_reader::ParseToBus(query);
					catalogue.AddBus(data_for_bus.name, data_for_bus.stops);
				}
			}

			for (const auto& query : input) {
				if (input_reader::IsStopQuery(query)) {
					auto distances_from_stop = input_reader::ParseToDistanceToStops(query);

					for (const auto& dictance_data : distances_from_stop) {
						catalogue.SetDistanceBetweenStops(dictance_data.stop_name_from,
							dictance_data.stop_name_to, dictance_data.distance);
					}
				}
			}
		}
	} // namespace input_reader
} // namespace catalogue 