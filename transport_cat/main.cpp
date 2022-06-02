#include <iostream>
#include <Windows.h>
#include <sstream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include "json_reader.h"

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    /*auto input = catalogue::input_reader::ParseQueriesToVector(cin);
    catalogue::input_reader::FillTransportCatalogue(catalogue, input);

    auto stat = catalogue::input_reader::ParseQueriesToVector(cin);
    catalogue::stat_reader::DisplayInformation(catalogue, stat);*/
    
    catalogue::TransportCatalogue tc;
       
    // Получаем данные, с помощью которых можно заполнить транспортный каталог
    auto data = catalogue::json_reader::GetDataFromJson(cin);
    // Заполняем каталог данными
    catalogue::json_reader::FillTransportCatalogue(tc, data);
    // Получаем данные в виде json для запроса
    std::stringstream strm;

    catalogue::json_reader::GetStatInfo(tc, data, strm);
    std::cout << strm.str();

    /*
    {
  "base_requests": [
    {
      "type": "Bus",
      "name": "114",
      "stops": ["Морской вокзал", "Ривьерский мост"],
      "is_roundtrip": false
    },
    {
      "type": "Stop",
      "name": "Ривьерский мост",
      "latitude": 43.587795,
      "longitude": 40.0,
      "road_distances": {"Морской вокзал": 850}
    },
    {
      "type": "Stop",
      "name": "Морской вокзал",
      "latitude": 43.581969,
      "longitude": 39.719848,
      "road_distances": {"Ривьерский мост": 850}
    }
  ],
  "stat_requests": [
    { "id": 1, "type": "Stop", "name": "Ривьерский мост" },
    { "id": 2, "type": "Bus", "name": "114" }
  ]
}
    */
}

// Получение информации из JSON
    // 1. Производим парсинг из запроса в JSON-формат:
    //  - получаем Document методом Load
    // 2. Проходимся по документy и определяем вид запроса:
    // 3. base_requests - массив с описанием автобусных маршрутов и остановок,
    // 4. stat_requests - массив с запросами к транспортному справочнику.
    //  3.1. Для base_requests определяем тип (ключ - "type")
    //  3.2. Stop - создание остановки (имя, координаты, расстояния до соседних)
    //  3.3. Bus - создание маршрута (имя, остановки, тип маршрута (круг или нет))
    //  
    //  4.1. Для stat_requests определяем тип (ключ - "type")
    //  4.2. Bus - инфо о маршруте:
    //      curvature,
    //      request_id (содержится в запросе, ключ - "id"),
    //      route_length,
    //      stop_count,
    //      unique_stop_count
    //  4.3. Stop - инфо об остановке:
    //      buses (отсортированный массив маршрутов через остановку),
    //      request_id  (содержится в запросе, ключ - "id")
