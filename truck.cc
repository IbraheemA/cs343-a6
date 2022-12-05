#pragma once

_Task Truck {
    Printer & prt;
    NameServer & nameServer;
    BottlingPlant & plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;

    unsigned int * cargo;

    int lastRestocked;

    void main() {
      VendingMachine ** vms = nameServer.getMachineList();
      for (;;) {
        yield(prng(1, 10));
        plant.getShipment(cargo);      
      }
    }
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
        prt{prt}, nameServer{nameServer}, plant{plant},
        numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {
          cargo = new int[numVendingMachines];
          lastRestocked = 0;
        }

};
