#pragma once

#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"

_Task Truck {
    Printer & printer;
    NameServer & nameServer;
    BottlingPlant & plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;

    unsigned int * cargo;

    int restockIndex;

    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};
