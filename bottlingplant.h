#pragma once

// class Printer;

#include "printer.h"
#include "nameserver.h"

// class NameServer;
static const int NUM_OF_FLAVOURS = 4;

_Task BottlingPlant {
    Printer & printer;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    uCondition shipment;
    bool shutdownHasHappened = false;

    unsigned int *nextShipment;
    void main();
  public:
    enum Flavours { Beef, BuhrBrew, OrangeBarley, ThumbsUp };
    _Event Shutdown {};					// shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};
