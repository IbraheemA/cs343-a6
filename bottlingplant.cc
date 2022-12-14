#include "bottlingplant.h"

#include "printer.h"
#include "truck.h"

#include <iostream>
#include <fstream>

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    Truck truck = Truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);

    for (;;) {
        _Accept(~BottlingPlant) {
            shutdownHasHappened = true;
            try {
                _Accept(getShipment);
            } catch( uMutexFailure::RendezvousFailure & ) {
                // XXX
            }
            break;
        } or _Accept(getShipment) {
            yield(timeBetweenShipments);
            int totalBottles = 0;
            for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
                int b = prng(maxShippedPerFlavour);
                nextShipment[i] = b;
                totalBottles += b;
            }
            printer.print(Printer::Kind::BottlingPlant, 'G', totalBottles);
        }
    }

    std::ofstream test_out{"t.out", std::ios::app};
    printer.print(Printer::Kind::BottlingPlant, 'F');
}

BottlingPlant::BottlingPlant(
    Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments
) :
    printer{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines},
    maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour},
    timeBetweenShipments{timeBetweenShipments}
{
    nextShipment = new unsigned int[NUM_OF_FLAVOURS]; // Length of Flavours
    for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
        nextShipment[i] = prng(maxShippedPerFlavour);
    }
}

BottlingPlant::~BottlingPlant() {
    delete [] nextShipment;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    std::ofstream test_out{"t.out", std::ios::app};
    if (shutdownHasHappened) {
        throw Shutdown();
    }

    for (int i = 0; i < NUM_OF_FLAVOURS; i++) {
        cargo[i] = nextShipment[i];
    }
    printer.print(Printer::Kind::BottlingPlant, 'P');
}
