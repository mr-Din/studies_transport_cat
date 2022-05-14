#include <iostream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

void FillTransportCatalogue(TransportCatalogue& catalogue, vector<string>& input) {
    // вектор с именами остановок из запросов (для добавления по ним расстояний)
    vector<string_view> names_of_stops;

    for (const auto& query : input) {
        if (input_reader::IsStopQuery(query)) {
            auto data_for_stop = input_reader::ParseToStop(query);
            catalogue.AddStop(data_for_stop.name, data_for_stop.coordinate);
            names_of_stops.push_back(data_for_stop.name);
        }
        else if (input_reader::IsBusQuery(query)) {
            auto data_for_bus = input_reader::ParseToBus(query);
            catalogue.AddBus(data_for_bus.name, data_for_bus.stops);
        }
    }

    // добавление информации о расстояниях между остановками
    for (const auto& query : input) {
        if (input_reader::IsStopQuery(query)) {
            // получаем из запроса Остановку и вектор пар vector<pair<double distance, string_view stop_to>>
            auto distances_from_stop = input_reader::ParseToDistanceToStops(query);
            // добавляем расстояния между остановками
            for (const auto& dictance_data : distances_from_stop) {

                catalogue.AddDistanceBetweenStops(dictance_data.stop_name_from,
                    dictance_data.stop_name_to, dictance_data.distance);

            }

        }
    }
}

void DisplayInformation(TransportCatalogue& catalogue, vector<string>& stat) {
    for (const auto& query : stat) {

        if (input_reader::IsBusQuery(query)) {
            auto bus_info = catalogue.GetBusInfo(stat_reader::ParseToName(query));
            stat_reader::OutputStat(bus_info);
        }
        else {
            auto stop_name = stat_reader::ParseToName(query);
            auto stop_info = catalogue.GetBusesForStop(stop_name);
            stat_reader::OutputBusesForStop(stop_name, stop_info);
        }
    }
}

int main()
{
    TransportCatalogue catalogue;

    /*catalogue.AddStop("Tolstopaltsevo"s, 55.611087, 37.208290);
    catalogue.AddStop("Marushkino"s, 55.595884, 37.209755);
    catalogue.AddStop("Rasskazovka"s, 55.632761, 37.333324);
    catalogue.AddStop("Biryulyovo Zapadnoye"s, 55.574371, 37.651700);
    catalogue.AddStop("Biryusinka"s, 55.581065, 37.648390);
    catalogue.AddStop("Universam"s, 55.587655, 37.645687);
    catalogue.AddStop("Biryulyovo Tovarnaya"s, 55.592028, 37.653656);
    catalogue.AddStop("Biryulyovo Passazhirskaya"s, 55.580999, 37.659164);

    auto& s1 = catalogue.FindStop("Tolstopaltsevo"s);
    auto& s2 = catalogue.FindStop("Marushkino"s);
    auto& s3 = catalogue.FindStop("Rasskazovka"s);
    auto& s4 = catalogue.FindStop("Biryusinka"s);
    auto& s5 = catalogue.FindStop("Universam"s);
    auto& s6 = catalogue.FindStop("Biryulyovo Tovarnaya"s);
    auto& s7 = catalogue.FindStop("Biryulyovo Passazhirskaya"s);
    vector<Stop> stops{ s1,s2,s3,s4,s5,s6,s7 };
    for (auto& s : stops) {
        cout << setprecision(6) << s.name << " - " << s.coordinates.lat << "; " << s.coordinates.lng << endl;
    }

    vector <string> stops256{
        "Biryulyovo Zapadnoye"s,
        "Biryusinka"s,
        "Universam"s,
        "Biryulyovo Tovarnaya"s,
        "Biryulyovo Passazhirskaya"s,
        "Biryulyovo Zapadnoye"s};
    vector <string> stops750{
        "Tolstopaltsevo"s,
        "Marushkino"s,
        "Rasskazovka"s,
        "Marushkino"s,
        "Tolstopaltsevo"s };

    catalogue.AddBus("256"s, stops256);
    catalogue.AddBus("750"s, stops750);*/

    // Вызов ввода числа запросов, самих запросов и заполнение базы запросов!
    auto input = input_reader::ParseQueriesToVector();
    sort(input.begin(), input.end(), std::greater<std::string>());
    FillTransportCatalogue(catalogue, input);
    
    auto stat = input_reader::ParseQueriesToVector();
    DisplayInformation(catalogue, stat);
}

/*

=======================Ввод данных========================

13
Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
Stop Prazhskaya: 55.611678, 37.603831
6
Bus 256
Bus 750
Bus 751
Stop Samara
Stop Prazhskaya
Stop Biryulyovo Zapadnoye


*/

/*

=======================Вывод данных========================

Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature
Bus 750: 7 stops on route, 3 unique stops, 27400 route length, 1.30853 curvature
Bus 751: not found
Stop Samara: not found
Stop Prazhskaya: no buses
Stop Biryulyovo Zapadnoye: buses 256 828

*/