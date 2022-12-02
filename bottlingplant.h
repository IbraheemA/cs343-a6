#pragma once

// class Printer;
// class NameServer;

// #include "printer.h"
// #include "nameserver.h"

_Task BottlingPlant {
    void main();
  public:
    enum Flavours { Beef, BuhrBrew, meme1, meme2 };	// flavours of soda (YOU DEFINE)
    _Event Shutdown {};					// shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};
