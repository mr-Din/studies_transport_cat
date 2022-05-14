#include <iostream>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

int main()
{
    catalogue::TransportCatalogue catalogue;

    catalogue::input_reader::FillTransportCatalogue(catalogue);
    catalogue::stat_reader::DisplayInformation(catalogue);
}