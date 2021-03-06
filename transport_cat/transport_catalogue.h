#pragma once
#include "geo.h"
#include "domain.h"

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

		void AddStop(std::string_view stop_name, const geo::Coordinates& coordinates);
		void AddBus(std::string_view bus_name,
			const std::vector<std::string_view>& stops_query, bool is_roundtrip);

		const Bus* FindBus(std::string_view bus_name) const;
		const Stop* FindStop(std::string_view stop_name) const;

		BusInfo GetBusInfo(std::string_view name) const;
		StopInfo GetBusesForStop(std::string_view stop_name) const;
		void SetDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to, double distance);
		double GetDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to) const;
		const std::deque<Bus> GetBuses() const;

	private:

		std::deque<Stop> stops_;
		std::unordered_map<std::string_view, const Stop*> stopname_to_stop_;
		std::deque<Bus> buses_;
		std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
		std::unordered_map<const Stop*, std::set<std::string_view>> stops_to_buses_;
		std::unordered_map<std::pair<const Stop*, const Stop*>, double, detail::PairStopsHasher> distance_;
	};
}