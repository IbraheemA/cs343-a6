#include "bottlingplant.h"

void BottlingPlant::main() {
    Truck truck = Truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);

    for (;;) {
        _Accept(getShipment) {
            yield(timeBetweenShipments);
            for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
                nextShipment[i] = prng(maxShippedPerFlavour);
            }
        }
    }
}

BottlingPlant::BottlingPlant(
    Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments
) :
    prt{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines},
    maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour},
    timeBetweenShipments{timeBetweenShipments}
{
    nextShipment = new unsigned int[NUM_OF_FLAVOURS]; // Length of Flavours
    for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
        nextShipment[i] = prng(maxShippedPerFlavour);
    }
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
        cargo[i] = nextShipment[i];
    }
}
