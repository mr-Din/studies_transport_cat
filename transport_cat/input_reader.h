#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "log_duration.h"
#include "transport_catalogue.h"

namespace catalogue {
	namespace input_reader {

		struct DataForStop {
			std::string_view name;
			Coordinates coordinate;
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
			std::string ReadLine();
			int ReadLineWithNumber();
			std::vector<std::string> ParseQueriesToVector();
		}

		bool IsStopQuery(std::string_view query);
		bool IsBusQuery(std::string_view query);

		DataForStop ParseToStop(std::string_view query);
		DataForBus ParseToBus(std::string_view query);
		std::string_view ParseToName(std::string_view query, size_t start_name, size_t colon_pos);
		std::pair<double, double> ParseToCoordinates(std::string_view query, size_t colon_pos);
		std::vector<std::string_view> ParseToStopsName(std::string_view query, size_t colon_pos);
		std::vector<DataForDistance> ParseToDistanceToStops(std::string_view query);

		void FillTransportCatalogue(TransportCatalogue& catalogue);
	}
}