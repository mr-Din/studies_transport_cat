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

            if (!query_map.at("is_roundtrip").AsBool()) {

                int size_data_for_bus = static_cast<int>(data_for_bus.stops.size());
                for (int i = size_data_for_bus; ; --i) {
                    if ((i - 2) < 0) { break; }
                    data_for_bus.stops.push_back(data_for_bus.stops[i - 2]);
                }
            }

            data_from_json.buses.push_back(std::move(data_for_bus));
        }

        void FillStatRequests(const json::Array& stat_queries, DataFromJson& data_from_json) {
            for (auto& query : stat_queries) {
                auto& query_map = query.AsMap();
                if (query_map.count("name"s)) {
                    data_from_json.stat_requests.push_back({
                        query_map.at("id"s).AsInt(),
                        query_map.at("name"s).AsString(),
                        query_map.at("type"s).AsString()
                        });
                }
                else {
                    data_from_json.stat_requests.push_back({
                        query_map.at("id"s).AsInt(),
                        ""s,
                        query_map.at("type"s).AsString()
                        });
                }
            }
        }

        void FillRenderRequests(const json::Dict& query_map, DataFromJson& data_from_json) {
            auto& render_settings = data_from_json.render_settings;

            for (const auto& [param, value] : query_map) {
                if (param == "width"s) { render_settings.width = value.AsDouble(); }
                else if (param == "height"s) { render_settings.height = value.AsDouble(); }
                else if (param == "padding"s) { render_settings.padding = value.AsDouble(); }
                else if (param == "stop_radius"s) { render_settings.stop_radius = value.AsDouble(); }
                else if (param == "line_width"s) { render_settings.line_width = value.AsDouble(); }
                else if (param == "bus_label_font_size"s) { render_settings.bus_label_font_size = value.AsInt(); }
                else if (param == "bus_label_offset"s) {
                    render_settings.bus_label_offset.x = value.AsArray()[0].AsDouble();
                    render_settings.bus_label_offset.y = value.AsArray()[1].AsDouble();
                }
                else if (param == "stop_label_font_size"s) { render_settings.stop_label_font_size = value.AsInt(); }
                else if (param == "stop_label_offset"s) {
                    render_settings.stop_label_offset.x = value.AsArray()[0].AsDouble();
                    render_settings.stop_label_offset.y = value.AsArray()[1].AsDouble();
                }

                else if (param == "underlayer_color"s) {
                    render_settings.underlayer_color = FormatColorForSvg(value);
                }
                else if (param == "underlayer_width"s) { render_settings.underlayer_width = value.AsDouble(); }
                else if (param == "color_palette"s) {
                    for (const auto& color : value.AsArray()) {
                        render_settings.color_palette.push_back(FormatColorForSvg(color));
                    }
                }
            }
        }


        DataFromJson GetDataFromJson(std::istream& input)
        {
            DataFromJson data_from_json;

            auto query_from_json = json::Load(input);
            auto& root = query_from_json.GetRoot();
            auto& root_map = root.AsMap();

            if (root_map.count("base_requests")) {
                auto& queries = root_map.at("base_requests").AsArray();

                for (auto& query : queries) {
                    auto& query_map = query.AsMap();

                    if (query_map.at("type").AsString() == "Stop") {
                        FillStopsFromQuery(query_map, data_from_json);
                    }

                    if (query_map.at("type").AsString() == "Bus") {
                        FillBusesFromQuery(query_map, data_from_json);
                    }
                }
            }
            

            if (root_map.count("stat_requests"s)) {
                auto& queries = root_map.at("stat_requests"s).AsArray();
                FillStatRequests(queries, data_from_json);
            }

            if (root_map.count("render_settings"s)) {              
                const auto& setting_map = root_map.at("render_settings"s).AsMap();
                FillRenderRequests(setting_map, data_from_json);    
            }

            return data_from_json;
        }

        void FillTransportCatalogue(catalogue::TransportCatalogue& tc, DataFromJson& data_from_json)
        {
            for (auto& stop : data_from_json.stops) {
                tc.AddStop(stop.name, stop.coordinate);
            }
            for (const auto& bus : data_from_json.buses) {
                tc.AddBus(bus.name, bus.stops);
            }
            for (const auto& dist : data_from_json.distances) {
                tc.SetDistanceBetweenStops(dist.stop_name_from, dist.stop_name_to, dist.distance);
            }
        }

        void PrintStatInfoAll(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out)
        {
            json::Array info;

            for (auto& stat_request : data_from_json.stat_requests) {
                if (stat_request.type == "Bus") {
                    info.emplace_back(std::move(GetStatInfoBus(tc, stat_request)));
                }

                if (stat_request.type == "Stop") {
                    info.emplace_back(std::move(GetStatInfoStop(tc, stat_request)));
                }

                if (stat_request.type == "Map") {
                    info.emplace_back(std::move(GetStatInfoRenderer(tc, data_from_json, stat_request)));
                }
            }
            json::Print(json::Document{ info }, out);
        }

        json::Dict GetStatInfoBus(const TransportCatalogue& tc, const StatRequest& stat_request) {
            auto bus_info = tc.GetBusInfo(stat_request.name);
            if (bus_info.stops_count == 0) {
                return json::Dict{
                    {"request_id"s, stat_request.id},
                    {"error_message"s, "not found"s}
                    };
            }
            else {
                return json::Dict{
                {"curvature"s, bus_info.curvature},
                {"request_id"s, stat_request.id},
                {"route_length"s, bus_info.distance},
                {"stop_count"s, static_cast<int>(bus_info.stops_count)},
                {"unique_stop_count"s,  static_cast<int>(bus_info.unique_stops_count)}
                    };
            }
        }

        json::Dict GetStatInfoStop(const TransportCatalogue& tc, const StatRequest& stat_request) {
            auto stop_info = tc.GetBusesForStop(stat_request.name);

            json::Array stops_to_buses_arr;
            stops_to_buses_arr.reserve(stop_info.stops_to_buses_.size());
            for (auto& s : stop_info.stops_to_buses_) {
                stops_to_buses_arr.emplace_back(std::string(s));
            }

            if (stop_info.name.empty()) {
                return json::Dict{
                    {"request_id"s, stat_request.id},
                    {"error_message"s, "not found"s}
                    };
            }
            else {
                return json::Dict{
                {"buses"s, stops_to_buses_arr},
                {"request_id"s, stat_request.id}
                    };
            }
        }

        json::Dict GetStatInfoRenderer(const TransportCatalogue& tc,
            const DataFromJson& data_from_json, const StatRequest& stat_request) {
            renderer::MapRenderer render(tc, data_from_json.render_settings);
            render.SetRenderBus();
            std::stringstream strm;
            render.Print(strm);

            return json::Dict{
                {"request_id"s, stat_request.id},
                {"map"s, strm.str()}
                };
        }

        std::string FormatColorForSvg(const json::Node& value)
        {
            if (value.IsArray()) {
                const auto& rgb_arr = value.AsArray();
                std::string rgb_str = ""s;
                if (rgb_arr.size() == 3) {
                    rgb_str = "rgb("s;
                }
                if (rgb_arr.size() == 4) {
                    rgb_str = "rgba("s;
                }
                for (size_t i = 0; i < rgb_arr.size(); ++i) {
                    if (i > 0) {
                        rgb_str += ',';
                    }
                    if (i != 3) {
                        std::ostringstream strs;
                        strs << rgb_arr[i].AsInt();
                        rgb_str += strs.str();
                    }
                    else {
                        std::ostringstream strs;
                        strs << rgb_arr[i].AsDouble();
                        rgb_str += strs.str();
                    }
                }
                rgb_str += ')';
                return rgb_str;
            }
            else {
                return value.AsString();
            }
        }

    }
}
