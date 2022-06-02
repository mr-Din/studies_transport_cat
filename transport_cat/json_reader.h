#pragma once
#include "json.h"
#include "transport_catalogue.h"

namespace catalogue {
    namespace json_reader {
        // Информация об остановке (имя и координаты)
        struct DataForStop {
            std::string name;
            geo::Coordinates coordinate;
        };
        // Информация о маршруте (имя, остановки(для передачи в виде string_view,), остановки (хранение строк))
        struct DataForBus {
            std::string name;
            std::vector<std::string_view> stops;
            std::deque<std::string> stops_str;
        };
        // Информация о расстояниях (имя остановки От, имя остановки До, расстояние)
        struct DataForDistance {
            std::string stop_name_from;
            std::string stop_name_to;
            int distance;
        };
        // Информация о запросах (id, имя, тип запроса)
        struct StatRequest {
            int id;
            std::string name;
            std::string type;
        };

        // Набор необходимых данных из JSON запроса
        struct DataFromJson {
            std::deque<DataForStop> stops;
            std::deque<DataForBus> buses;
            std::deque<DataForDistance> distances;
            std::deque<StatRequest> stat_requests;
        };
        // Функция возвращающая DataFromJson
        DataFromJson GetDataFromJson(std::istream& input);

        // Заполнение остановок
        void FillStopsFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);

        // Заполнение маршрутов
        void FillBusesFromQuery(const json::Dict& query_map, DataFromJson& data_from_json);


        // Функция заполняющая данными транспортный каталог (принимает каталог по ссылке, заполняе из DataFromJson)
        void FillTransportCatalogue(TransportCatalogue& tc, DataFromJson& data_from_json);

        // Ответ на запрос
        void GetStatInfo(TransportCatalogue& tc, DataFromJson& data_from_json, std::ostream& out);
    }
}