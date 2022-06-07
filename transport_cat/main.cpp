#include <iostream>
#include <Windows.h>
#include <sstream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "json_reader.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    catalogue::TransportCatalogue tc;

    auto data = catalogue::json_reader::GetDataFromJson(std::cin);
    // Заполняем каталог данными
    catalogue::json_reader::FillTransportCatalogue(tc, data);
    // Получаем данные
    catalogue::json_reader::PrintStatInfoAll(tc, data, std::cout);
}
