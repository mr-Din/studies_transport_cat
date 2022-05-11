#include "transport_catalogue.h"

using namespace std;


void TransportCatalogue::AddStop(const std::string& stop_name, const double lat, const double lng)
{
	// добавляем остановку в stops_
	stops_.push_back({ stop_name, {lat, lng} });
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

void TransportCatalogue::AddBus(const std::string& bus_name,
	const std::vector<std::string>& stops_query) {

	// маршрут (последовательность остановок)
	vector<const Stop*> bus;
	bus.reserve(stops_query.size());
	for_each(
		stops_query.begin(), stops_query.end(),
		[&](const string& stop_name) {
			bus.push_back(stopname_to_stop_[stop_name]);
			//bus.push_back(&FindStop(stop_name));
		});
	/*for (auto& stop_name : stops_query) {
		bus.push_back(&FindStop(stop_name));
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

const Bus& TransportCatalogue::FindBus(const std::string& bus_name)
{
	static Bus empty_res;

	auto iter = busname_to_bus_.find(bus_name);
	if (iter == busname_to_bus_.end()) {
		return empty_res;
	}
	return *iter->second;
}

const Stop& TransportCatalogue::FindStop(const std::string& stop_name)
{
	static Stop empty_res;

	auto iter = stopname_to_stop_.find(stop_name);
	if (iter == stopname_to_stop_.end()) {
		return empty_res;
	}
	return *iter->second;
}

std::tuple<size_t, size_t, double> TransportCatalogue::GetBusInfo(const std::string& bus_name)
{
	auto& bus = FindBus(bus_name);
	size_t stops_count = bus.bus.size();
	double length = bus.distance;

	// уникальные значения через set
	std::unordered_set<const Stop*> unique_stops(bus.bus.begin(), bus.bus.end());
	size_t unique_stops_count = unique_stops.size();
	return { stops_count, unique_stops_count, length };
}




