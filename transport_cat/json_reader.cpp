#include "json_reader.h"
using namespace std::literals;
namespace catalogue {
    namespace json_reader {
        void FillStopsFromQuery(const json::Dict& query_map, DataFromJson& data_from_json)
        {
            DataForStop data_for_stop;

            data_for_stop.name = query_map.at("name").AsString();
            data_for_stop.coordinate.lat = query_map.at("latitude").AsDouble();
            data_for_stop.coordinate.lng = query_map.at("longitude").AsDouble();
            data_from_json.stops.push_back(std::move(data_for_stop));

            // необходимо учесть road_distance
            auto& distance_map = query_map.at("road_distances").AsMap();
            for (auto& [to_stop, dist] : distance_map) {

                data_from_json.distances.push_back({
                    data_from_json.stops.back().name,
                    to_stop, dist.AsInt() });
            }
        }

        void FillBusesFromQuery(const json::Dict& query_map, DataFromJson& data_from_json)
        {
            DataForBus data_for_bus;

            data_for_bus.name = query_map.at("name").AsString();

            for (auto& stops_of_bus : query_map.at("stops").AsArray()) {

                data_for_bus.stops_str.push_back(stops_of_bus.AsString());
                data_for_bus.stops.push_back(data_for_bus.stops_str.back());
            }

            // Если маршрут не круговой: ABC (ABCBA)
            if (!query_map.at("is_roundtrip").AsBool()) {

                int size_data_for_bus = static_cast<int>(data_for_bus.stops.size());
                for (int i = size_data_for_bus; ; --i) {
                    if ((i - 2) < 0) { break; }
                    data_for_bus.stops.push_back(data_for_bus.stops[i - 2]);
                }
            }

            data_from_json.buses.push_back(std::move(data_for_bus));
        }


        DataFromJson GetDataFromJson(std::istream& input)
        {
            DataFromJson data_from_json;
            // получаем json документ из входящих данных (cin)
            auto query_from_json = json::Load(input);
            // получаем корневой элемент
            auto& root = query_from_json.GetRoot();

            auto& root_map = root.AsMap();

            // Запрос на заполнение данными
            if (root_map.count("base_requests")) {

                auto& queries = root_map.at("base_requests").AsArray();

                for (auto& query : queries) {
                    auto& query_map = query.AsMap();
                    // запрос на создание остановки
                    if (query_map.at("type").AsString() == "Stop") {
                        FillStopsFromQuery(query_map, data_from_json);
                    }


                    // запрос на создание маршрута
                    if (query_map.at("type").AsString() == "Bus") {
                        FillBusesFromQuery(query_map, data_from_json);
                    }
                }
            }


            if (root_map.count("stat_requests")) {
                auto& queries = root_map.at("stat_requests").AsArray();
                for (auto& query : queries) {
                    auto& query_map = query.AsMap();
                    data_from_json.stat_requests.push_back({
                        query_map.at("id").AsInt(),
                        query_map.at("name").AsString(),
                        query_map.at("type").AsString()
                        });
                }
            }

            return data_from_json;
        }

        void FillTransportCatalogue(catalogue::TransportCatalogue& tc, DataFromJson& data_from_json)
        {
            // добавление остановок
            for (auto& stop : data_from_json.stops) {
                tc.AddStop(stop.name, stop.coordinate);
            }
            // добавление маршрутов
            for (const auto& bus : data_from_json.buses) {
                tc.AddBus(bus.name, bus.stops);
            }
            // добавление расстояний между остановками
            for (const auto& dist : data_from_json.distances) {
                tc.SetDistanceBetweenStops(dist.stop_name_from, dist.stop_name_to, dist.distance);
            }
        }

        void GetStatInfo(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out)
        {
            json::Array info;
            // Получение информации по stat_requests
            for (auto& stat_request : data_from_json.stat_requests) {
                if (stat_request.type == "Bus") {
                    auto bus_info = tc.GetBusInfo(stat_request.name);
                    // Если маршрут не найден:
                    if (bus_info.stops_count == 0) {
                        info.emplace_back(json::Dict{
                            {"request_id"s, stat_request.id},
                            {"error_message"s, "not found"s}
                            });
                    }
                    else {
                        info.emplace_back(json::Dict{
                        {"curvature"s, bus_info.curvature},
                        {"request_id"s, stat_request.id},
                        {"route_length"s, bus_info.distance},
                        {"stop_count"s, static_cast<int>(bus_info.stops_count)},
                        {"unique_stop_count"s,  static_cast<int>(bus_info.unique_stops_count)}
                            });
                    }

                }
                if (stat_request.type == "Stop") {
                    auto stop_info = tc.GetBusesForStop(stat_request.name);

                    json::Array stops_to_buses_arr;
                    stops_to_buses_arr.reserve(stop_info.stops_to_buses_.size());
                    for (auto& s : stop_info.stops_to_buses_) {
                        stops_to_buses_arr.emplace_back(std::string(s));
                    }

                    if (stop_info.name.empty()) {
                        info.emplace_back(json::Dict{
                            {"request_id"s, stat_request.id},
                            {"error_message"s, "not found"s}
                            });
                    }
                    else {
                        info.emplace_back(json::Dict{
                        {"buses"s, stops_to_buses_arr},
                        {"request_id"s, stat_request.id}
                            });
                    }

                }
            }

            json::Print(json::Document{ info }, out);
        }

    }
}