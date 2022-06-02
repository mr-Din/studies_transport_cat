#pragma once
#include "json.h"
#include "transport_catalogue.h"

namespace catalogue {
    namespace json_reader {
        // ���������� �� ��������� (��� � ����������)
        struct DataForStop {
            std::string name;
            geo::Coordinates coordinate;
        };
        // ���������� � �������� (���, ���������(��� �������� � ���� string_view,), ��������� (�������� �����))
        struct DataForBus {
            std::string name;
            std::vector<std::string_view> stops;
            std::deque<std::string> stops_str;
        };
        // ���������� � ����������� (��� ��������� ��, ��� ��������� ��, ����������)
        struct DataForDistance {
            std::string stop_name_from;
            std::string stop_name_to;
            int distance;
        };
        // ���������� � �������� (id, ���, ��� �������)
        struct StatRequest {
            int id;
            std::string name;
            std::string type;
        };

        // ����� ����������� ������ �� JSON �������
        struct DataFromJson {
            std::deque<DataForStop> stops;
            std::deque<DataForBus> buses;
            std::deque<DataForDistance> distances;
            std::deque<StatRequest> stat_requests;
        };
        // ������� ������������ DataFromJson
        DataFromJson GetDataFromJson(std::istream& input);

        // ���������� ���������
        void FillStopsFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);

        // ���������� ���������
        void FillBusesFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);


        // ������� ����������� ������� ������������ ������� (��������� ������� �� ������, �������� �� DataFromJson)
        void FillTransportCatalogue(TransportCatalogue& tc, DataFromJson& data_from_json);

        // ����� �� ������
        void GetStatInfo(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out);
    }
}