#include "transport_catalogue.h"

using namespace std;


namespace catalogue {
	void TransportCatalogue::AddInputQueries(std::vector<std::string> query) {
		input_queries_ = move(query);
		sort(input_queries_.begin(), input_queries_.end(), std::greater<std::string>());
	}

	void TransportCatalogue::AddStatQueries(std::vector<std::string> query) {
		stat_queries_ = move(query);
	}

	const std::vector<std::string>& TransportCatalogue::GetInputQueries() const {
		return input_queries_;
	}

	const std::vector<std::string>& TransportCatalogue::GetStatQueries() const {
		return stat_queries_;
	}

	void TransportCatalogue::AddStop(string_view stop_name, const Coordinates coordinates)
	{
		stops_.push_back({ stop_name, { coordinates.lat, coordinates.lng} });

		stopname_to_stop_[stops_.back().name] = &stops_.back();

		stops_to_buses_[&stops_.back()];
	}

	void TransportCatalogue::AddBus(string_view bus_name,
		const vector<string_view>& stops_query) {

		vector<const Stop*> bus;
		bus.reserve(stops_query.size());
		for_each(
			stops_query.begin(), stops_query.end(),
			[&](auto stop_name) {
				bus.push_back(stopname_to_stop_[stop_name]);
			});

		buses_.push_back({ bus_name, bus });

		busname_to_bus_[buses_.back().name] = &buses_.back();

		AddBusToStops(&buses_.back());
	}

	const Bus* TransportCatalogue::FindBus(std::string_view bus_name)
	{
		static Bus* empty_res;

		auto iter = busname_to_bus_.find(bus_name);
		if (iter == busname_to_bus_.end()) {
			return empty_res;
		}
		return iter->second;
	}

	const Stop* TransportCatalogue::FindStop(std::string_view stop_name)
	{
		static Stop* empty_res;

		auto iter = stopname_to_stop_.find(stop_name);
		if (iter == stopname_to_stop_.end()) {
			return empty_res;
		}
		return iter->second;
	}

	BusInfo TransportCatalogue::GetBusInfo(std::string_view bus_name)
	{
		auto bus = FindBus(bus_name);
		int distance = 0;
		double curvature = 0.0;
		double geo_distance = 0.0;
		size_t stops_count = 0;
		size_t unique_stops_count = 0;
		if (bus) {
			stops_count = bus->bus.size();
			for (size_t i = 0; i < stops_count - 1;++i) {
				geo_distance += ComputeDistance(bus->bus[i]->coordinates, bus->bus[i + 1]->coordinates);
				distance += GetDistanceBetweenStops(bus->bus[i]->name, bus->bus[i + 1]->name);
			}
			curvature = static_cast<double>(distance) / geo_distance;

			std::unordered_set<const Stop*> unique_stops(bus->bus.begin(), bus->bus.end());
			unique_stops_count = unique_stops.size();
		}
		return { bus_name, stops_count, unique_stops_count, distance, curvature };
	}

	StopInfo TransportCatalogue::GetBusesForStop(std::string_view stop_name)
	{
		static StopInfo empty_res;
		auto stop = FindStop(stop_name);

		auto iter = stops_to_buses_.find(stop);
		if (iter == stops_to_buses_.end()) {
			return empty_res;
		}
		return { stop_name, iter->second };
	}

	void TransportCatalogue::AddDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to, int distance)
	{
		auto stop_from = FindStop(stop_name_from);
		auto stop_to = FindStop(stop_name_to);
		if (stop_from && stop_to) {
			distance_[{stop_from, stop_to}] = distance;
			if (!distance_.count({ stop_to, stop_from })) {
				distance_[{stop_to, stop_from}] = distance;
			}
		}
	}

	int TransportCatalogue::GetDistanceBetweenStops(std::string_view stop_name_from, std::string_view stop_name_to)
	{
		auto iter = distance_.find({ FindStop(stop_name_from), FindStop(stop_name_to) });
		if (iter == distance_.end()) {
			return 0.0;
		}
		return iter->second;
	}

	void TransportCatalogue::AddBusToStops(const Bus* added_bus) {
		for (auto stop : added_bus->bus) {
			stops_to_buses_.at(stop).insert(added_bus->name);
		}
	}
}