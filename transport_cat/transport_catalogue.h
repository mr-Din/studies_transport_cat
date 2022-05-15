#pragma once
#include "geo.h"
#include "log_duration.h"

#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <utility>

namespace catalogue {
	struct Stop {
		std::string_view name;
		Coordinates coordinates;
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
	
	namespace detail {
		struct PairStopsHasher {
			size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const {
				return hasher(stops.first) + hasher(stops.first) * 37;
			}
			std::hash<const void*> hasher;
		};
	}

	class TransportCatalogue {
	public:

		void AddStop(std::string_view stop_name, const Coordinates& coordinates);
		void AddBus(std::string_view bus_name,
			const std::vector<std::string_view>& stops_query);

		const Bus* FindBus(std::string_view bus_name);
		const Stop* FindStop(std::string_view stop_name);

		BusInfo GetBusInfo(std::string_view name);
		StopInfo GetBusesForStop(std::string_view stop_name);
		void SetDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to, int distance);
		int GetDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to);


	private:

		std::deque<Stop> stops_;
		std::unordered_map<std::string_view, const Stop*> stopname_to_stop_;
		std::deque<Bus> buses_;
		std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
		std::unordered_map<const Stop*, std::set<std::string_view>> stops_to_buses_;
		std::unordered_map<std::pair<const Stop*, const Stop*>, int, detail::PairStopsHasher> distance_;

	};
}