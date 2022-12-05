#include "truck.h"

void Truck::main() {
  try {
    VendingMachine ** vms = nameServer.getMachineList();
    for (;;) {
      yield(prng(1, 10));
      plant.getShipment(cargo);

      for (int i = 0; i < numVendingMachines; i++) {
        unsigned int inventory[NUM_OF_FLAVOURS] = vms[restockIndex].inventory();
        int inventoryCount = 0;
        for (int j = 0; j < NUM_OF_FLAVOURS; j++) {
          if (cargo[j] > 0 && inventory[j] < maxStockPerFlavour) {
            int newInventory = max(inventory[j] + cargo[j], maxStockPerFlavour);
            cargo[j] = inventory[j] + cargo[j] - newInventory;
            inventory[j] = newInventory;
            inventoryCount += cargo[j];
          }
        }
        if (prng(100) == 0) {
          yield(10);
        }
        vms[restockIndex++].restocked();      
        if (inventoryCount == 0) {
          break;
        }
      }    
    }
  } catch(Shutdown e) {}
}

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    prt{prt}, nameServer{nameServer}, plant{plant},
  numVendingMachines{numVendingMachines}, maxStockPerFlavour{maxStockPerFlavour} {
    cargo = new int[numVendingMachines];
    restockIndex = 0;
}
