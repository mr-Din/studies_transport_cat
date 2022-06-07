#pragma once
#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include <sstream>

namespace catalogue {
    namespace json_reader {

        struct DataForStop {
            std::string name;
            geo::Coordinates coordinate;
        };

        struct DataForBus {
            std::string name;
            std::vector<std::string_view> stops;
            std::deque<std::string> stops_str;
        };

        struct DataForDistance {
            std::string stop_name_from;
            std::string stop_name_to;
            int distance;
        };

        struct StatRequest {
            int id;
            std::string name;
            std::string type;
        };

        struct DataFromJson {
            std::deque<DataForStop> stops;
            std::deque<DataForBus> buses;
            std::deque<DataForDistance> distances;
            std::deque<StatRequest> stat_requests;
            renderer::RenderSettings render_settings;
        };

        DataFromJson GetDataFromJson(std::istream& input);

        void FillStopsFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);

        void FillBusesFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);

        void FillStatRequests(const json::Array& stat_queries, DataFromJson& data_from_json);

        void FillRenderRequests(const json::Dict& query_map, DataFromJson& data_from_json);

        void FillTransportCatalogue(TransportCatalogue& tc, DataFromJson& data_from_json);

        void PrintStatInfoAll(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out);

        json::Dict GetStatInfoBus(const TransportCatalogue& tc, const StatRequest& stat_request);
        json::Dict GetStatInfoStop(const TransportCatalogue& tc, const StatRequest& stat_request);
        json::Dict GetStatInfoRenderer(const TransportCatalogue& tc, const DataFromJson& data_from_json, const StatRequest& stat_request);

        std::string FormatColorForSvg(const json::Node& value);
    }
}