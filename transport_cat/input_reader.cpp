#include "input_reader.h"

using namespace std;

namespace input_reader {
	string ReadLine() {
		string s;
		getline(cin, s);
		return s;
	}

	int ReadLineWithNumber() {
		int result;
		cin >> result;
		ReadLine();
		return result;
	}

	// формируем вектор из запросов
	vector<string> ParseQueriesToVector() {
		int query_count = ReadLineWithNumber();
		vector<string> queries;
		queries.reserve(query_count);
		for (int i = 0; i < query_count; ++i) {
			string query = ReadLine();
			queries.push_back(move(query));
		}
		return queries;
	}


	InputReader Load()
	{
		vector<string> queries = ParseQueriesToVector();

		// —оздали структуру, в которой буду располагатьс€ запросы
		InputReader input;

		// ќбрабатываем каждый запрос (string query) и заполн€ем
		// нужный контейнер структуры

		for (string& query : queries) {
			// ≈сли запрос на создание остановки
			if (!query.empty() && query[0] == 'S') {
				size_t start_name = query.find_first_not_of(' ', 4);
				size_t colon_pos = query.find_first_of(':');
				size_t comma_pos = query.find_first_of(',');
				
				input.stops_query_[move(ParseToName(query, start_name, colon_pos))]
					= ParseToCoordinates(query, colon_pos, comma_pos);
			}
			// ≈сли запрос на создание маршрута
			else if (!query.empty() && query[0] == 'B') {
				size_t start_name = query.find_first_not_of(' ', 3);
				size_t colon_pos = query.find_first_of(':');

				input.buses_query_[move(ParseToName(query, start_name, colon_pos))] =
					ParseToStopsName(query, colon_pos);
			}
		}

		return input;
	}

	string ParseToName(const string& query, size_t start_name, size_t colon_pos)
	{
		string stop_name = query.substr(
			start_name,
			query.find_last_not_of(' ', colon_pos - 1) - start_name + 1);
		//std::cout <<'!'<< stop_name<<'!' << std::endl;
		return stop_name;
		
	}

	pair<double, double> ParseToCoordinates(const string& query,
		size_t colon_pos, size_t comma_pos) {
		double lat = stod(query.substr(
			colon_pos + 1,
			comma_pos - colon_pos
		));
		double lng = stod(query.substr(
			comma_pos + 1
		));
		return { lat, lng };;
	}

	vector<string> ParseToStopsName(string& query, size_t colon_pos)
	{
		vector<string> stops;
		
		if (size_t separator_pos = query.find_first_of(">-"); separator_pos != string::npos) {
			size_t start_stop_name_pos = query.find_first_not_of(' ', colon_pos + 1);
			while (separator_pos != string::npos) {
				string stop_name = query.substr(
					start_stop_name_pos,
					query.find_last_not_of(' ', separator_pos - 1) - start_stop_name_pos + 1);
				stops.push_back(move(stop_name));

				start_stop_name_pos = query.find_first_not_of(' ', separator_pos + 1);
				separator_pos = query.find_first_of(">-", separator_pos + 1);
			}
			string stop_name = query.substr(
				start_stop_name_pos,
				query.find_last_not_of(' ') - start_stop_name_pos + 1);
			stops.push_back(move(stop_name));
		}
		// ≈сли остановки заданы форматом A - B - C
		if (query.find('-') != string::npos) {
			for (size_t i = stops.size() - 1; i > 0; --i) {
				stops.push_back(stops[i - 1]);
			}
		}
		return stops;
	}
}