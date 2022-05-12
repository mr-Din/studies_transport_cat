#include <iostream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

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
    {    
        LOG_DURATION("AddStopAndBus");
        for (const auto& query : input) {
            if (input_reader::IsStopQuery(query)) {
                auto data_for_stop = input_reader::ParseToStop(query);
                catalogue.AddStop(data_for_stop.name, data_for_stop.coordinate);
            }
            else if (input_reader::IsBusQuery(query)) {
                auto data_for_bus = input_reader::ParseToBus(query);
                catalogue.AddBus(data_for_bus.name, data_for_bus.stops);
            }
        }
    }

    
    auto stat = input_reader::ParseQueriesToVector();
    {
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
}

/*

=======================Ввод данных========================

13
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
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

Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found
Stop Samara: not found
Stop Prazhskaya: no buses
Stop Biryulyovo Zapadnoye: buses 256 828

*/