#include "truck.h"


#include <iostream>
#include <fstream>

void Truck::main() {
  printer.print(Printer::Kind::Truck, 'S');
  try {
    VendingMachine ** vms = nameServer.getMachineList();
    for (;;) {
      std::ofstream test_out{"t.out", std::ios::app};
      // std::osacquire(/**/std::cout) << "trucking along" << std::endl;
      yield(prng(1, 10));
      plant.getShipment(cargo);
      int inventoryCount = 0;
      for (int i = 0; i < NUM_OF_FLAVOURS; ++i) {
        inventoryCount += cargo[i];
      }
      printer.print(Printer::Kind::Truck, 'P', inventoryCount);

      
      for (int i = 0; i < numVendingMachines; i++) {
        int totalBottlesMissing = 0;
        printer.print(Printer::Kind::Truck, 'd', restockIndex, inventoryCount);
        unsigned int * inventory = vms[restockIndex]->inventory();
        for (int j = 0; j < NUM_OF_FLAVOURS; j++) {
          if (cargo[j] > 0 && inventory[j] < maxStockPerFlavour) {
            int newInventory = std::min(inventory[j] + cargo[j], maxStockPerFlavour);
            totalBottlesMissing += maxStockPerFlavour - newInventory;
            int bottlesUnloaded = newInventory - inventory[j];
            inventoryCount -= bottlesUnloaded;
            cargo[j] -= bottlesUnloaded;
            inventory[j] = newInventory;
          }
        }
        if (totalBottlesMissing > 0) {
          printer.print(Printer::Kind::Truck, 'U', restockIndex, totalBottlesMissing);
        } else {
          printer.print(Printer::Kind::Truck, 'D', restockIndex, inventoryCount);
        }
        if (prng(100) == 0) {
          printer.print(Printer::Kind::Truck, 'X');
          yield(10);
        }
        vms[restockIndex]->restocked();
        restockIndex = (restockIndex + 1) % numVendingMachines;  
        if (inventoryCount == 0) {
          break;
        }
      }    
    }
  } catch(BottlingPlant::Shutdown e) {}

  std::ofstream test_out{"t.out", std::ios::app};
  // std::osacquire(/**/std::cout) << "sweet release" << std::endl;
  printer.print(Printer::Kind::Truck, 'F');
}

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    printer{prt}, nameServer{nameServer}, plant{plant},
  numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {
    cargo = new unsigned int[numVendingMachines];
    restockIndex = 0;
}

Truck::~Truck() {
  delete [] cargo;
}
