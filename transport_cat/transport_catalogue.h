#pragma once
#include "geo.h"
#include "log_duration.h"

#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <utility>




struct Stop {
	std::string_view name;
	Coordinates coordinates;
};
struct Bus {	// маршрут
	std::string_view name;
	std::vector<const Stop*> bus;
	double distance;
};

struct BusInfo {
	size_t stops_count;
	size_t unique_stops_count;
	double distance;
};

struct PairStopsHasher {
	size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const {
		return hasher(std::string(stops.first->name) + std::string(stops.second->name));
	}
	std::hash<std::string> hasher;
};

class TransportCatalogue {
public:

	//добавление остановки в базу,
	void AddStop(const std::string_view stop_name, const Coordinates coordinates);
	//добавление маршрута в базу,
	void AddBus(std::string_view bus_name,
		const std::vector<std::string_view>& stops_query);
	//поиск маршрута по имени,
	const Bus& FindBus(std::string_view bus_name);
	//поиск остановки по имени,
	const Stop& FindStop(std::string_view stop_name);
	//получение информации о маршруте.
	BusInfo GetBusInfo(std::string_view name);
	


private:
	
	// все остановки:
	std::deque<Stop> stops_;
	// имя остановки-указатель на остановку:
	std::unordered_map<std::string_view, const Stop*> stopname_to_stop_; 
	// все маршруты:
	std::deque<Bus> buses_;
	// имя маршрута-указатель на маршрут:
	std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
	// расстояние между остановками:
	std::unordered_map<std::pair<const Stop*, const Stop*>, double, PairStopsHasher> distance_;
};
