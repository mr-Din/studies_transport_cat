#include <iostream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

int main()
{
    catalogue::TransportCatalogue catalogue;

    auto input = catalogue::input_reader::ParseQueriesToVector(cin);
    catalogue::input_reader::FillTransportCatalogue(catalogue, input);

    auto stat = catalogue::input_reader::ParseQueriesToVector(cin);
    catalogue::stat_reader::DisplayInformation(catalogue, stat);
}