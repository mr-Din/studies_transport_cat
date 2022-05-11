#include "transport_catalogue.h"

using namespace std;


void TransportCatalogue::AddStop(const string_view stop_name, const Coordinates coordinates)
{
	// добавляем остановку в stops_
	stops_.push_back({ stop_name, { coordinates.lat, coordinates.lng} });
	// добавляем остановку в stopname_to_stop_
	stopname_to_stop_[stops_.back().name] = &stops_.back();
	// добавление расстояния до других остановок

	for (const auto& [name, stop] : stopname_to_stop_) {
		distance_[{&stops_.back(), stop}] =
			ComputeDistance(stops_.back().coordinates, stop->coordinates);
		distance_[{stop,  &stops_.back() }] =
			ComputeDistance(stop->coordinates, stops_.back().coordinates);
	}

}

void TransportCatalogue::AddBus(string_view bus_name,
	const vector<string_view>& stops_query) {
	// маршрут (последовательность остановок)
	vector<const Stop*> bus;
	bus.reserve(stops_query.size());
	for_each(
		stops_query.begin(), stops_query.end(),
		[&](auto stop_name) {
			bus.push_back(stopname_to_stop_[stop_name]);
		});
	/*for (auto stop_name : stops_query) {
		bus.push_back(stopname_to_stop_[stop_name]);
	}*/

	// расстояние маршрута
	double distance = 0.0;
	//for (auto it = bus.begin(); it != bus.end()-1; ++it) {
	for (size_t i = 0; i < bus.size() - 1;++i) {
		//distance += distance_.at({ *it, *(it+1) });
		distance += distance_[{bus[i], bus[i + 1]}];
	}
	buses_.push_back({ bus_name, bus, distance });

	// добавление индекса (имя маршрута - указатель на маршрут)
	busname_to_bus_[buses_.back().name] = &buses_.back();
}

const Bus& TransportCatalogue::FindBus(std::string_view bus_name)
{
	static Bus empty_res;

	auto iter = busname_to_bus_.find(bus_name);
	if (iter == busname_to_bus_.end()) {
		return empty_res;
	}
	return *iter->second;
}

const Stop& TransportCatalogue::FindStop(std::string_view stop_name)
{
	static Stop empty_res;

	auto iter = stopname_to_stop_.find(stop_name);
	if (iter == stopname_to_stop_.end()) {
		return empty_res;
	}
	return *iter->second;
}

BusInfo TransportCatalogue::GetBusInfo(std::string_view bus_name)
{
	auto& bus = FindBus(bus_name);
	size_t stops_count = bus.bus.size();
	double length = bus.distance;

	// уникальные значения через set
	std::unordered_set<const Stop*> unique_stops(bus.bus.begin(), bus.bus.end());
	size_t unique_stops_count = unique_stops.size();
	return { stops_count, unique_stops_count, length };
}




