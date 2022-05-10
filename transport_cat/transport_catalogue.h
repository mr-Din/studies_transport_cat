#pragma once
#include "geo.h"

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
	std::string name;
	Coordinates coordinates;
};
struct Bus {	// маршрут
	std::string name;
	std::vector<const Stop*> bus;
	double distance;
};

struct PairStopsHasher {
	size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const {
		return hasher(stops.first->name + stops.second->name);
	}
	std::hash<std::string> hasher;
};

class TransportCatalogue {
public:

	//добавление остановки в базу,
	void AddStop(const std::string& stop_name, const double lat, const double lng);
	//добавление маршрута в базу,
	void AddBus(const std::string& bus_name,
		const std::vector<std::string>& stops_query);
	//поиск маршрута по имени,
	const Bus& FindBus(const std::string& bus_name);
	//поиск остановки по имени,
	const Stop& FindStop(const std::string& stop_name);
	//получение информации о маршруте.
	std::tuple<size_t, size_t, double> GetBusInfo(const std::string& name);
	


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
