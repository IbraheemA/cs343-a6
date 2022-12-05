#pragma once

// class Printer;
// class NameServer;

// #include "printer.h"
// #include "nameserver.h"

static const int NUM_OF_FLAVOURS = 4;

_Task BottlingPlant {
    void main();
  public:
    enum Flavours { Beef, BuhrBrew, OrangeBarley, ThumbsUp };
    _Event Shutdown {};					// shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};
