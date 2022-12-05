#pragma once

_Task Truck {
    Printer & prt;
    NameServer & nameServer;
    BottlingPlant & plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;

    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
        prt{prt}, nameServer{nameServer}, plant{plant},
        numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {}

};
