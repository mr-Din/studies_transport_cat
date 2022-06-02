#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "transport_catalogue.h"

namespace catalogue {
	namespace input_reader {

		struct DataForStop {
			std::string_view name;
			geo::Coordinates coordinate;
		};
		struct DataForBus {
			std::string_view name;
			std::vector<std::string_view> stops;
		};
		struct DataForDistance {
			std::string_view stop_name_from;
			std::string_view stop_name_to;
			int distance;
		};

		namespace detail {
			std::string ReadLine(std::istream& input_query);
			int ReadLineWithNumber(std::istream& input_query);
		}

		std::vector<std::string> ParseQueriesToVector(std::istream& input_query);

		bool IsStopQuery(std::string_view query);
		bool IsBusQuery(std::string_view query);

		DataForStop ParseToStop(std::string_view query);
		DataForBus ParseToBus(std::string_view query);
		std::string_view ParseToName(std::string_view query, size_t start_name, size_t colon_pos);
		geo::Coordinates ParseToCoordinates(std::string_view query, size_t colon_pos);
		std::vector<std::string_view> ParseToStopsName(std::string_view query, size_t colon_pos);
		std::vector<DataForDistance> ParseToDistanceToStops(std::string_view query);

		void FillTransportCatalogue(TransportCatalogue& catalogue, std::vector<std::string>& input);
	}
}