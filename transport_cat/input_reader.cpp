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

	// ��������� ������ �� ��������
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

	bool IsStopQuery(string_view query)
	{
		return !query.empty() && query[0] == 'S';
	}

	bool IsBusQuery(string_view query)
	{
		return !query.empty() && query[0] == 'B';
	}

	DataForStop ParseToStop(string_view query)
	{
		size_t start_name = query.find_first_not_of(' ', 4);
		size_t colon_pos = query.find_first_of(':');
		auto coordinates = ParseToCoordinates(query, colon_pos);
		return { ParseToName(query, start_name, colon_pos),
			 coordinates.first, coordinates.second };
	}

	DataForBus ParseToBus(string_view query)
	{
		size_t start_name = query.find_first_not_of(' ', 3);
		size_t colon_pos = query.find_first_of(':');

		return { ParseToName(query, start_name, colon_pos),
			ParseToStopsName(query, colon_pos) };
	}
	
	string_view ParseToName(string_view query, size_t start_name, size_t colon_pos)
	{
		string_view stop_name = query.substr(
			start_name,
			query.find_last_not_of(' ', colon_pos - 1) - start_name + 1);
		//std::cout <<'!'<< stop_name<<'!' << std::endl;
		return stop_name;
		
	}

	pair<double, double> ParseToCoordinates(string_view query,
		size_t colon_pos) {
		// ������� ������ ������� � ������� (����������� ��������� lat � lng)
		size_t comma_pos = query.find_first_of(',');
		// ������ ���������� ����� colon_pos ':' � ������ ������� ','
		double lat = stod(string(query).substr(
			colon_pos + 1,
			comma_pos - colon_pos
		));
		// ������ ���������� ����� ������ � ������ �������
		size_t comma_pos_next = query.find_first_of(',', comma_pos + 1);
		double lng = stod(string(query).substr(
			comma_pos + 1, comma_pos_next
		));
		return { lat, lng };
	}

	vector<string_view> ParseToStopsName(string_view query, size_t colon_pos)
	{
		vector<string_view> stops;
		
		if (size_t separator_pos = query.find_first_of(">-"); separator_pos != string::npos) {
			size_t start_stop_name_pos = query.find_first_not_of(' ', colon_pos + 1);
			while (separator_pos != string::npos) {
				string_view stop_name = query.substr(
					start_stop_name_pos,
					query.find_last_not_of(' ', separator_pos - 1) - start_stop_name_pos + 1);
				stops.push_back(move(stop_name));

				start_stop_name_pos = query.find_first_not_of(' ', separator_pos + 1);
				separator_pos = query.find_first_of(">-", separator_pos + 1);
			}
			string_view stop_name = query.substr(
				start_stop_name_pos,
				query.find_last_not_of(' ') - start_stop_name_pos + 1);
			stops.push_back(move(stop_name));
		}
		// ���� ��������� ������ �������� A - B - C
		if (query.find('-') != string::npos) {
			for (size_t i = stops.size() - 1; i > 0; --i) {
				stops.push_back(stops[i - 1]);
			}
		}
		return stops;
	}
	vector<DataForDistance> ParseToDistanceToStops(std::string_view query)
	{
		vector<DataForDistance> distances;
		// 1. ������� ������� ����� �������� ��������� (�������� �� 2� �������)
		// 2. ������� ����� m � �� �� � �������� ������� ���� ������ ��� ������� (�������� ����� m � �������� ��� ������� - ����������)
		// 3. ������� ����� m ������� to (������� find � ������� m)
		// 4. � ����� ������� to ���� ������ ������������ ������ � ������ ������ �������
		// 5. ����� ���� ����� ��� ��������� �� ������� ������� ����������
		// 6. ��������� ��� � ������
		// 
		// ��������� ���� � ������� ��������� ��������� �������. ���� ������� �� �����, ���� ������� npos
		size_t start_name = query.find_first_not_of(' ', 4);
		size_t colon_pos = query.find_first_of(':');
		auto stop_name_from = ParseToName(query, start_name, colon_pos);
		
		size_t pos_begin = query.find_first_of(',', query.find_first_of(',') + 1) + 1;	 // 1
		pos_begin = query.find_first_not_of(' ', pos_begin);
		while (pos_begin != string::npos && pos_begin != 0) {
			size_t pos_end = query.find_first_of('m', pos_begin);
			int distance = stoi(string(query).substr(pos_begin, pos_end - pos_begin));	// 2

			pos_begin = query.find_first_not_of(' ', query.find("to"s, pos_end) + 2);
			pos_end = query.find_last_not_of(' ', query.find_first_of(',', pos_end) - 1);
			string_view stop_name_to = query.substr(pos_begin, pos_end - pos_begin + 1);

			pos_begin = query.find_first_not_of(' ', query.find_first_of(',', pos_end) + 1);
			distances.push_back({ stop_name_from, stop_name_to, distance });
		}
		
		// Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam

		return distances;
	}

}