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




struct Stop {
	std::string_view name;
	Coordinates coordinates;
};
struct Bus {	// маршрут
	std::string_view name;
	std::vector<const Stop*> bus;
	//double distance;
};

struct BusInfo {
	std::string_view name;
	size_t stops_count;
	size_t unique_stops_count;
	double distance;
};

struct StopInfo {
	std::string_view name;
	std::set<std::string_view> stops_to_buses_;
};

struct PairStopsHasher {
	size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const {
		return hasher(stops.first) + hasher(stops.first)*37;
	}
	std::hash<const void*> hasher;
};

class TransportCatalogue {
public:

	//добавление остановки в базу,
	void AddStop(const std::string_view stop_name, const Coordinates coordinates);
	//добавление маршрута в базу,
	void AddBus(std::string_view bus_name,
		const std::vector<std::string_view>& stops_query);
	//поиск маршрута по имени,
	const Bus* FindBus(std::string_view bus_name);
	//поиск остановки по имени,
	const Stop* FindStop(std::string_view stop_name);
	//получение информации о маршруте.
	BusInfo GetBusInfo(std::string_view name);
	StopInfo GetBusesForStop(std::string_view stop_name);


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
	//std::unordered_map<std::pair<const Stop*, const Stop*>, double, PairStopsHasher> distance_;
	// ключ - остановка -> значение - маршруты этой остановки
	//std::unordered_map<const Stop*, std::vector<const Bus*>> stops_to_buses_;
	std::unordered_map<const Stop*, std::set<std::string_view>> stops_to_buses_;

	void AddBusToStops(const Bus*);
};
