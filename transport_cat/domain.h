#pragma once
#include "geo.h"

#include<set>
#include<vector>
#include<string_view>

namespace catalogue {

	struct Stop {
		std::string_view name;
		geo::Coordinates coordinates;
	};
	struct Bus {
		std::string_view name;
		std::vector<const Stop*> bus;
	};

	struct BusInfo {
		std::string_view name;
		size_t stops_count;
		size_t unique_stops_count;
		int distance;
		double curvature;
	};

	struct StopInfo {
		std::string_view name;
		std::set<std::string_view> stops_to_buses_;
	};
}